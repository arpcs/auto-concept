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

#include "clang/Sema/DeclSpec.h"
#include "clang/Sema/Initialization.h"
#include "clang/Sema/Lookup.h"
#include "clang/Sema/Overload.h"
#include "clang/Sema/ParsedTemplate.h"
#include "clang/Sema/Scope.h"
#include "clang/Sema/SemaInternal.h"
#include "clang/Sema/Template.h"
#include "clang/Sema/TemplateDeduction.h"

#include "ProbeManager.h"

namespace auto_concept {
    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;
    using namespace std;

    MatchHandler::RewriterPointer MatchHandler::createRewriter(clang::DiagnosticsEngine& DiagnosticsEngine,
        clang::ASTContext& Context) {
        auto Rewriter =
            std::make_unique<clang::FixItRewriter>(DiagnosticsEngine,
                Context.getSourceManager(),
                Context.getLangOpts(),
                &FixItOptions);

        // Note: it would make more sense to just create a raw pointer and have the
        // DiagnosticEngine own it. However, the FixItRewriter stores a pointer to
        // the client of the DiagnosticsEngine when it gets constructed with it.
        // If we then set the rewriter to be the client of the engine, the old
        // client gets destroyed, leading to happy segfaults when the rewriter
        // handles a diagnostic.
        DiagnosticsEngine.setClient(Rewriter.get(), /*ShouldOwnClient=*/false);

        

        return Rewriter;
    }

    void MatchHandler::run(const MatchFinder::MatchResult& Result) {

        if (this->customMatchHandler) {
            this->customMatchHandler(Result);
            return;
        }

        ASTContext* Context = Result.Context;
        auto& map = Result.Nodes.getMap();

        for (auto& m : map) {
            if (CLOptions::LogLevelOption >= 3) {
                llvm::outs() << "Match found: " << m.first << "\n";
                m.second.dump(llvm::outs(), *Context);
                llvm::outs() << "Match end: " << m.first << "\n";
            }
        }

        if (const auto* func = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplateDecl rewrite")) 
            matches[func->getID()].push_back(Result);

        if (const auto* func = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplateDecl2"))
            matches[func->getID()].push_back(Result);
    }

    void MatchHandler::onStartOfTranslationUnit() {
        matches.clear();
    }

    void MatchHandler::onEndOfTranslationUnit() {      

        RewriterPointer Rewriter = nullptr;
        if (DoRewrite && matches.size() > 0) {
            ASTContext* firstContext = matches.begin()->second.begin()->Context;
            if (firstContext != nullptr) {
                auto& DiagnosticsEngine = firstContext->getDiagnostics();
                Rewriter = createRewriter(DiagnosticsEngine, *firstContext);
            }
        }  
        if (this->tuState == AutoConceptTuState::InjectingProbes) InjectProbes(matches, resources);
        else if (this->tuState == AutoConceptTuState::CollectingResults) CollectProbes(matches, guessers);
        else if (this->tuState == AutoConceptTuState::ActingOnResults)
        for (auto& matchesPair : matches) {
            auto firstMatch = *matchesPair.second.begin();
            ASTContext* firstContext = firstMatch.Context;

            if (const auto* func = firstMatch.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplateDecl rewrite")) {
                const auto& funcDecl = func->getTemplatedDecl();

                FullSourceLoc FullLocation = firstContext->getFullLoc(funcDecl->getBeginLoc());
                FullLocation.getColumnNumber();
                SourceLocation s = funcDecl->getBeginLoc();

                string replaceText = "requires ";
                string noteText = "";
                int i = 0;
                for (auto & match : matchesPair.second) {
                    if (const auto* param = match.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("templateTypeParmDecl arithmetic")) {
                        const std::string& paramName = param->getName().str();
                        if (i++ != 0) replaceText += "&& ";
                        if (i != 0) noteText += " ";
                        replaceText += "std::is_arithmetic_v<" + paramName + "> ";
                        noteText += "(arithmetic)'" + paramName + "'";
                    }
                }
                
                // ToDo: \t....
                auto FixIt = FixItHint::CreateInsertion(funcDecl->getBeginLoc(), replaceText + "\n\t");
                auto& diag = firstContext->getDiagnostics();
                const auto diagID = diag.getCustomDiagID(clang::DiagnosticsEngine::Remark, "Consider adding concepts to template(s): %0");
                // So we destroy our builder to execute it..
                {
                    const auto& builder = diag.Report(func->getBeginLoc(), diagID);
                    builder.AddString(noteText);
                    builder.AddFixItHint(FixIt);
                }
                
            }
            
            if (auto* funcTemp = const_cast<clang::FunctionTemplateDecl*>(firstMatch.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplateDecl2"))) {

                Guesser guesser;
                auto fullName = funcTemp->getCanonicalDecl()->getQualifiedNameAsString();
                if (guessers && guessers.get()->find(fullName) != guessers.get()->end())
                    guesser = guessers.get()->at(fullName);
                if (funcTemp->getTemplateParameters()) {
                    for (const auto& param : *funcTemp->getTemplateParameters()) 
                        guesser.templateParams.push_back(param->getName().str());

                    for (auto spec : funcTemp->specializations()) {
                        if (CLOptions::LogLevelOption >= 3) spec->dumpColor();
                        auto specParams = spec->getTemplateSpecializationArgs();
                        Guesser::SpecTypes specParamObj;
                        for (const auto& param : specParams->asArray()) {
                            auto fullType = param.getAsType().getCanonicalType().getAsString();
                            /*while (true) {
                                auto pos = fullType.find("std::");
                                if (pos == string::npos) break;
                                fullType = fullType.substr(0, pos) + fullType.substr(pos + "std::"s.size());
                            }
                            while (true) {
                                auto pos = fullType.find("class");
                                if (pos == string::npos) break;
                                fullType = fullType.substr(0, pos) + fullType.substr(pos + "class"s.size());
                            }
                            while (true) {
                                auto pos = fullType.find(' ');
                                if (pos == string::npos) break;
                                fullType = fullType.substr(0, pos) + fullType.substr(pos + " "s.size());
                            }*/
                            specParamObj.types.push_back(fullType);
                        }

                        specParamObj.good = !spec->isInvalidDecl();
                        guesser.templateSpecs.insert(specParamObj);
                    }

                    if (CLOptions::TestConceptOption.size() > 0) llvm::outs() << "[Test concept] Testing function: " << fullName << "\n";
                    auto fitting = guesser.GetFittingConcepts(resources);
                    
                    // Rewriting

                    const auto& funcDecl = funcTemp->getTemplatedDecl();

                    FullSourceLoc FullLocation = firstContext->getFullLoc(funcDecl->getBeginLoc());
                    FullLocation.getColumnNumber();
                    SourceLocation s = funcDecl->getBeginLoc();

                    string replaceText = "requires ";
                    string noteText = "";
                    int i = 0;
                    for (auto f : fitting)
                    {
                        if (f) {
                            const std::string& paramName = f->templateParamNames;
                            if (i++ != 0) {
                                replaceText += " && ";
                                noteText += " ";
                            }
                            replaceText += f->conc.name+"<" + paramName + ">";
                            noteText += "("+f->conc.name + ")'" + paramName + "'";
                        }
                    }
                    if (i != 0) {
                        // ToDo: \t....
                        auto FixIt = FixItHint::CreateInsertion(funcDecl->getBeginLoc(), replaceText + "\n\t");
                        auto& diag = firstContext->getDiagnostics();

                        const auto diagID = diag.getCustomDiagID(clang::DiagnosticsEngine::Remark, "Consider adding concepts to template(s): %0");
                        // So we destroy our builder to execute it..
                        {
                            const auto& builder = diag.Report(funcTemp->getBeginLoc(), diagID);
                            builder.AddString(noteText);
                            builder.AddFixItHint(FixIt);
                        }
                    }
                }
            }
            
        }
        
        if (DoRewrite && Rewriter != nullptr) Rewriter->WriteFixedFiles();
    }

}