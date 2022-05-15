
#include "ResourceTypes.h"
#include "AutoConcept.h"

#include <string>
#include <fstream>

namespace auto_concept {

    void FillMissingResources(Resources resources) {

        //RunAppOnVirtual(input);
    }
    /*std::vector<std::string> GetMatchers() {
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
                buffer += line.substr(startPos, endPos - 1);
            }
            else if (startPos != std::string::npos && endPos == std::string::npos) {
                reading = true;
                buffer += line.substr(startPos);
            }
            else if (reading && endPos != std::string::npos) {
                reading = false;
                buffer += line.substr(0, endPos);
                result.push_back(buffer.substr(0, buffer.length() - 1));
            }
            else if (reading) {
                buffer += line;
            }
        }
        return result;
    }*/
}