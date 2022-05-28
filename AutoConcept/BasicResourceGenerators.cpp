
#include "ResourceTypes.h"
#include "AutoConcept.h"

#include <string>
#include <fstream>
#include <concepts>
#include <ranges>
#include <type_traits>
#include <iterator>
#include <unordered_map>
#include <unordered_set>

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <algorithm> 

#include "ConsoleHelpers.h"

namespace auto_concept {
    using namespace llvm;
    using namespace std;
    using namespace clang::ast_matchers;
    using namespace std;

    void FillBasicResourceParameters(Resources& resources) {

        string input = R"(
            #include <concepts>
            #include <type_traits>
            #include <ranges>
            #include <iterator>)";

        ProgressBar conceptProgressBar("Filling basic concept parameters", 
            std::count_if(resources.concepts.begin(), resources.concepts.end(), [](Concept& r) { return r.isConcept; }));
        unordered_map<string, int> conceptNames;
        {
            int i = 0;
            for (const auto& conceptRow : resources.concepts) conceptNames[conceptRow.name] = i++;
        }
        for (auto& conceptRow : resources.concepts) conceptRow.found = false;

        RunApp(input, {},
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

        ProgressBar templateProgressBar("Filling basic template predicate parameters",
            std::count_if(resources.concepts.begin(), resources.concepts.end(), [](Concept& r) { return !r.isConcept; }));
        RunApp(input, {},
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
                llvm::outs() << "Concept or template predicate not found: " << conceptRow.name << "\n";
            }
            else if (conceptRow.numberOfArguments == 0 || conceptRow.numberOfArguments > 3) {
                llvm::outs() << "Concept or template predicate with few or too many number of required arguments: " << conceptRow.name << " : " << conceptRow.numberOfArguments << "\n";
            }
        }
        llvm::outs() << "\n";
    }

    void FillResourceRelationships(Resources& resources) {

        string baseIncludes = R"(
            #include <concepts>
            #include <type_traits>
            #include <ranges>
            #include <iterator>
            using namespace std;
            using namespace ranges;)";

        for (auto& row : resources.concepts) row.passingTypes.clear();

        ProgressBar conceptProgressBar("Testing concepts",
            std::count_if(resources.concepts.begin(), resources.concepts.end(), [](Concept& r) { return r.found && r.numberOfArguments==1; }) * resources.types.size() );

        unordered_map<string, int> includesMap;
        for (const auto& typesRow : resources.types) {
            if (typesRow.include.size() > 0) includesMap[typesRow.include] = 1;
        }
        for (const auto& conceptsRow : resources.concepts) {
            if (!conceptsRow.found) continue;
            if (conceptsRow.include.size() > 0) includesMap[conceptsRow.include] = 1;
        }

        unordered_map<int, int> IDToConcept, IDToType;
        int ID = 0, conceptInd = 0;
        string conceptTesters;
        for (const auto& conceptsRow : resources.concepts) {
            int typeInd = 0;
            if (conceptsRow.numberOfArguments >= 1)  for (const auto& typesRow : resources.types) {
                if (conceptsRow.numberOfArguments == 1) {
                    IDToConcept[ID] = conceptInd;
                    IDToType[ID] = typeInd;
                    conceptTesters += "\n{constexpr bool testVal" + to_string(ID++) + " = " + conceptsRow.name + "<" + typesRow.name + ">; }";
                }
                typeInd++;
            }
            conceptInd++;
        }

        string conceptIncludes = baseIncludes;
        for (auto& conceptIncludeRow : includesMap) conceptIncludes+="\n#include <"+ conceptIncludeRow.first+">";

        string finalConceptInput = conceptIncludes + "\n void foo(){\n" + conceptTesters+"\n }";
        RunApp(finalConceptInput, {},
            [&]() { return varDecl(isConstexpr(), isExpansionInMainFile()).bind("VarDecl"); },
            [&](const MatchFinder::MatchResult& result) {
                if (const auto* conceptDec = result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
                    if (auto evVal = conceptDec->evaluateValue()) {
                        if (evVal->isInt()) {
                            auto value = evVal->getInt();
                            if (value.getBoolValue() == false)
                            {
                            }else if (value.getBoolValue() == true)
                            {
                                int ID = stoi(conceptDec->getDeclName().getAsString().substr(string("testVal").size()));
                                resources.concepts[IDToConcept[ID]].passingTypes.insert(
                                    resources.types[IDToType[ID]].name);
                            }
                            conceptProgressBar.Tick();
                        }
                    }
                }
            }
        );
        conceptProgressBar.End();
    }

    void FillMissingResources(Resources& resources) {
        FillBasicResourceParameters(resources);
        FillResourceRelationships(resources);
        resources.Save();
        int h = 5;
    }
}