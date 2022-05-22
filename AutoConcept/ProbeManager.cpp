#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Basic/SourceLocation.h"

#include "llvm/Support/CommandLine.h"


// Clang includes
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Rewrite/Frontend/FixItRewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/StandaloneExecution.h"
#include "clang/ASTMatchers/Dynamic/Parser.h"
#include "clang/Basic/CharInfo.h"
#include "clang/Tooling/NodeIntrospection.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "clang/AST/ASTImporter.h"

// LLVM includes
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

// Standard includes
#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <optional>
#include <iostream>
#include <concepts>

#include "CommandLine.h"
#include "MatchHandler.h"
#include "Guesser.h"
#include "ResourceTypes.h"
#include "ProbeManager.h"

#include "clang/Sema/DeclSpec.h"
#include "clang/Sema/Initialization.h"
#include "clang/Sema/Lookup.h"
#include "clang/Sema/Overload.h"
#include "clang/Sema/ParsedTemplate.h"
#include "clang/Sema/Scope.h"
#include "clang/Sema/SemaInternal.h"
#include "clang/Sema/Template.h"
#include "clang/Sema/TemplateDeduction.h" 

namespace auto_concept {
    using namespace clang;
    using namespace std;
    using MatchFinder = clang::ast_matchers::MatchFinder;
    using MatchResult = MatchFinder::MatchResult;

    void InjectProbes(const std::unordered_map< int64_t, clang::SmallVector<MatchResult> >& matches, std::shared_ptr<Resources> resources) {
        int probeIndex = 0;

        // Writing includes
        if (matches.size() > 0 && matches.begin()->second.size() > 0 && matches.begin()->second.begin()->Context != nullptr) {
            auto& diag = matches.begin()->second.begin()->Context->getDiagnostics();
            string includeTxt;
            unordered_set<string> includes;
            SourceLocation fileStartPos = diag.getSourceManager().getLocForStartOfFile(diag.getSourceManager().getMainFileID());

            for (const auto& type : resources->types) includes.insert(type.include);
            for (const auto& include : includes)
            {
                if (include.size() == 0) continue;
                includeTxt += "#include<" + include + ">\n";
            }
            // Since we changed the number of lines
            includeTxt += "#line 1\n";
            auto FixItInclude = FixItHint::CreateInsertion(fileStartPos, includeTxt);
            const auto diagIDInclude = diag.getCustomDiagID(clang::DiagnosticsEngine::Remark, "Writing AutoConcept include for probes");
            {
                const auto& builder = diag.Report(fileStartPos, diagIDInclude);
                builder.AddFixItHint(FixItInclude);
            }
        }

        for (auto& matchesPair : matches) {
            auto firstMatch = *matchesPair.second.begin();
            ASTContext* firstContext = firstMatch.Context;

            if (auto* funcTemp = const_cast<clang::FunctionTemplateDecl*>(firstMatch.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplateDecl2"))) {
                if (funcTemp->getTemplateParameters() != nullptr) {
                    auto tParams = funcTemp->getTemplateParameters();
                    const unsigned int minRequiredTemplateArguments = tParams->getMinRequiredArguments();
                    const unsigned int minRequiredFunctionArguments = funcTemp->getAsFunction()->getMinRequiredArguments();

                    const TemplateArgumentList* correctParams = nullptr;

                    for (auto spec : funcTemp->specializations()) {
                        if (!spec->isInvalidDecl()) {
                            correctParams = spec->getTemplateSpecializationArgs();
                            break;
                        }
                    }

                    // This is an arbitrary limitation to reduce complexity
                    if (correctParams == nullptr && minRequiredTemplateArguments > 1) continue;

                    string startText = "\nnamespace AutoConceptProbing" + to_string(probeIndex++) +"{\n\tvoid tester(){\n";
                    string inEveryLine = "\t\t";
                    string endText = "\t\t}\n\t}";
                    string lines;
                    int i = 0;
                    auto WriteLine = [&](vector< string > &args) {
                        lines += inEveryLine;
                        string qName = funcTemp->getQualifiedNameAsString();   
                        lines += qName + "<";
                        bool firstArg = true;
                        for (const auto& arg : args) {
                            if (!firstArg) lines += ",";
                            lines += arg;
                            firstArg = false;
                        }
                        lines += ">(";
                        for (int i = 0; i < minRequiredFunctionArguments; i++)
                        {
                            if (i!=0) lines += ",";
                            lines += "{}";
                        }
                        lines += ");\n";
                    };
                    

                    vector< string > testTempArgsBase;
                    if (correctParams != nullptr) {
                        for (const auto& correctParam : correctParams->asArray())
                        {
                            string type = correctParam.getAsType().getCanonicalType().getAsString();
                            testTempArgsBase.push_back(type);
                        }
                    }
                    else testTempArgsBase.resize(minRequiredTemplateArguments);

                    for (int argNum = 0; argNum < minRequiredTemplateArguments; argNum++)
                    {
                        for (const auto& typeToInsert : resources->types)
                        {
                            vector< string > testTempArgs = testTempArgsBase;
                            testTempArgs[argNum] = typeToInsert.name;
                            WriteLine(testTempArgs);
                        }
                    }

                    // Since we changed the number of lines
                    auto endLoc = funcTemp->getEndLoc().getLocWithOffset(1);
                    FullSourceLoc FullLocation = firstContext->getFullLoc(endLoc);
                    string lineNumText = "\n#line "+ to_string(FullLocation.getLineNumber() ) +"\n";

                    string finalText = startText + lines + endText + lineNumText;

                    //finalText = "asdfgg";



                    auto FixIt = FixItHint::CreateInsertion(endLoc, finalText);

                    auto& diag = firstContext->getDiagnostics();
                    const auto diagID = diag.getCustomDiagID(clang::DiagnosticsEngine::Remark, "Writing AutoConcept probe");
                    {
                        const auto& builder = diag.Report(endLoc, diagID);
                        builder.AddFixItHint(FixIt);
                    }

                   

                }
            }
        }
    }
}