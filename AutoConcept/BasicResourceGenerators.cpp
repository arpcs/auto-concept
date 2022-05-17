
#include "ResourceTypes.h"
#include "AutoConcept.h"

#include <string>
#include <fstream>
#include <concepts>
#include <ranges>
#include <type_traits>
#include <iterator>
#include <unordered_map>

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <algorithm> 

#include "ConsoleHelpers.h"

namespace auto_concept {
    using namespace llvm;
    using namespace std;
    using namespace clang::ast_matchers;
    using namespace std;

    void FillNumberOfConceptArguments(Resources& resources) {

        string input = R"(
            #include <concepts>
            #include <type_traits>
            #include <ranges>
            #include <iterator>)";

        ProgressBar conceptProgressBar("Filling basic concept parameters", 
            std::count_if(resources.concepts.begin(), resources.concepts.end(), [](Concepts& r) { return r.isConcept; }));
        unordered_map<string, int> conceptNames;
        {
            int i = 0;
            for (const auto& conceptRow : resources.concepts) conceptNames[conceptRow.name] = i++;
        }
        for (auto& conceptRow : resources.concepts) conceptRow.found = false;

        RunAppOnVirtual(input,
            [&]() { return decl().bind("decl"); },
            [&](const MatchFinder::MatchResult& result) {
                if (const auto* conceptDec = result.Nodes.getNodeAs<clang::ConceptDecl>("decl")) {
                    auto nodeName = conceptDec->getDeclName().getAsString();
                    auto conceptLoc = conceptNames.find(nodeName);
                    if (conceptLoc != conceptNames.end()) {
                        auto tParams= conceptDec->getTemplateParameters();
                        resources.concepts[conceptLoc->second].numberOfArguments = tParams->getMinRequiredArguments();
                        resources.concepts[conceptLoc->second].found = true;
                        conceptProgressBar.Tick();
                    }
                }
            }
        );
        conceptProgressBar.End();

        ProgressBar templateProgressBar("Filling basic template predictate parameters",
            std::count_if(resources.concepts.begin(), resources.concepts.end(), [](Concepts& r) { return !r.isConcept; }));
        RunAppOnVirtual(input,
            [&]() { return decl( has(templateTypeParmDecl()), has(varDecl(isConstexpr(), hasType(booleanType())))).bind("varTempDec"); },
            [&](const MatchFinder::MatchResult& result) {
                if (const auto* conceptDec = result.Nodes.getNodeAs<clang::VarTemplateDecl>("varTempDec")) {
                    auto nodeName = conceptDec->getDeclName().getAsString();
                    auto conceptLoc = conceptNames.find(nodeName);
                    if (conceptLoc == conceptNames.end() && nodeName.ends_with("_v")) conceptLoc = conceptNames.find(nodeName.substr(0, nodeName.size() - 2));
                    if (conceptLoc != conceptNames.end()) {
                        auto tParams = conceptDec->getTemplateParameters();
                        resources.concepts[conceptLoc->second].numberOfArguments = tParams->getMinRequiredArguments();
                        resources.concepts[conceptLoc->second].name = nodeName;
                        resources.concepts[conceptLoc->second].found = true;
                        templateProgressBar.Tick();
                    }
                }
            }
        );
        templateProgressBar.End();

        for (const auto& conceptRow : resources.concepts) {
            if (!conceptRow.found) {
                llvm::errs() << "Concept or template predictate not found: " << conceptRow.name << "\n";
            }
            else if (conceptRow.numberOfArguments == 0 || conceptRow.numberOfArguments > 3) {
                llvm::errs() << "Concept or template predictate with few or too many number of required arguments: " << conceptRow.name << " : " << conceptRow.numberOfArguments << "\n";
            }
        }
        int h = 54;
    }

    void FillMissingResources(Resources& resources) {
        FillNumberOfConceptArguments(resources);
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