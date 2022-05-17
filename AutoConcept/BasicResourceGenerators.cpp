
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
        
        unordered_map<string, int> conceptNames;
        {
            int i = 0;
            for (const auto& conceptRow : resources.concepts) conceptNames[conceptRow.name] = i++;
        }

        RunAppOnVirtual(input,
            [&]() { return decl().bind("decl"); },
            [&](const MatchFinder::MatchResult& result) {
                if (const auto* conceptDec = result.Nodes.getNodeAs<clang::ConceptDecl>("decl")) {
                    auto conceptName = conceptDec->getDeclName().getAsString();
                    auto conceptLoc = conceptNames.find(conceptName);
                    if (conceptLoc != conceptNames.end()) {
                        auto tParams= conceptDec->getTemplateParameters();
                        resources.concepts[conceptLoc->second].numberOfArguments = tParams->getMinRequiredArguments();
                    }
                }
            }
        );
        RunAppOnVirtual(input,
            [&]() { return decl( optionally(forEach(varDecl( has( cxxBoolLiteral().bind("lol")) ))) ,has(templateTypeParmDecl()), has(varDecl(isConstexpr(), hasType(booleanType())))).bind("varTemp"); },
            [&](const MatchFinder::MatchResult& result) {
                if (const auto* conceptDec = result.Nodes.getNodeAs<clang::VarTemplateDecl>("varTemp")) {
                    auto conceptName = conceptDec->getDeclName().getAsString();
                    auto conceptLoc = conceptNames.find(conceptName);
                    if (conceptLoc == conceptNames.end() && conceptName.ends_with("_v")) conceptLoc = conceptNames.find(conceptName.substr(0, conceptName.size() - 2));
                    if (conceptLoc != conceptNames.end()) {
                        if (conceptName == "is_array_v") {
                            conceptDec->dumpColor();
                            //auto tParams = conceptDec->getTemplateParameters()->getMinRequiredArguments();
                        }
                        auto specs = conceptDec->specializations();
                        for (auto spec : specs)
                        {
                            auto loool = spec->getInit();
                            auto typp = loool->getType();
                            auto typp2= loool->getType().getAsString();

                            auto typpk = *(spec->getDescribedVarTemplate()->getTemplateParameters()->begin());
                            auto tasdf = typpk->getDeclName().getAsString();
                       
                            auto tryy3 = static_cast<clang::CXXBoolLiteralExpr*>(loool);
                            //auto tryy3 = typp->getAs<clang::CXXBoolLiteralExpr>();
                            auto tryy4 = tryy3->getValue();
                            int h = 5;
                        }
                        if (const auto* conceptDec = result.Nodes.getNodeAs<clang::CXXBoolLiteralExpr>("lol")) {
                            auto map = result.Nodes.getMap();
                            auto value = conceptDec->getValue();
                            int i = 3;
                        }
                        //conceptDec->dumpColor();
                        auto tParams = conceptDec->getTemplateParameters();
                        resources.concepts[conceptLoc->second].numberOfArguments = tParams->getMinRequiredArguments();
                        //int h = 54;
                    }
                }
            }
        );
        for (const auto& conceptRow : resources.concepts) {
            if (conceptRow.numberOfArguments == 0 || conceptRow.numberOfArguments > 3) {
                int h = 2;
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