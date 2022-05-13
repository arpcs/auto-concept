#pragma once

#include <vector>

// Linking is pretty slow and we have to change matchers frequently
// So to mitigate this we use this file to build matchers dynamically from string or we just pass the matchers
// This is controlled by the AUTOCONCEPT_FAST_DYNAMIC_BUILD macro.
// We turn on intellisense on this file nevertheless to still get errors
#if defined(__INTELLISENSE__) || !defined(AUTOCONCEPT_FAST_DYNAMIC_BUILD)
#   include "clang/ASTMatchers/ASTMatchers.h"
    namespace auto_concept {
        std::vector<clang::ast_matchers::DeclarationMatcher> GetMatchers();
    }
#else
#   include <string>
#   include <fstream>
    namespace auto_concept {
        inline std::vector<std::string> GetMatchers(){
            std::vector<std::string> result;
            std::ifstream file("../AutoConcept/MatchersDeduced.cpp");
            if (!file.is_open()) {
                llvm::errs() << "ERROR: Can't open matcher file!\n";
                return {};
            }
            std::string line, buffer;
            bool reading = false;
            while (std::getline(file, line)) {

                auto startPos = line.find("matchers.push_back(");
                if (startPos != std::string::npos) startPos += std::string("matchers.push_back(").length();
                auto endPos = line.find(";");

                if (startPos != std::string::npos && endPos != std::string::npos) {
                    buffer += line.substr(startPos, endPos-1);
                }
                else if (startPos != std::string::npos && endPos == std::string::npos) {
                    reading = true;
                    buffer += line.substr(startPos);
                }
                else if (reading && endPos != std::string::npos) {
                    reading = false;
                    buffer += line.substr(0, endPos);
                    result.push_back( buffer.substr(0, buffer.length()-1 ) );
                }
                else if (reading) {
                    buffer += line;
                }
            }
            return result;
        }
    }
#endif