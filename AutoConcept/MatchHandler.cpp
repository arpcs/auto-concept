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

#include <iostream>
#include <concepts>

#include "CommandLine.h"
#include "MatchHandler.h"

namespace auto_concept {
    //Bad!
    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;

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
        ASTContext* Context = Result.Context;
        auto& map = Result.Nodes.getMap();


        for (auto& m : map) {
            llvm::outs() << "Match found: " << m.first << "\n";
            llvm::outs() << "rewriting=" << DoRewrite << "\tsuffix=" << deletethis << "\n";
            m.second.dump(llvm::outs(), *Context);
        }

        if (const auto* fun = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("function-to-autoconcept")) {
            const auto& body = fun->getAsFunction()->getBody();
            llvm::outs() << "body pointer: " << body << "\n";
            FullSourceLoc FullLocation = Context->getFullLoc(fun->getBeginLoc());
            llvm::outs() << "Found function-to-autoconcept at " << FullLocation.getSpellingLineNumber() << ":"
                << FullLocation.getSpellingColumnNumber() << "\n";

            auto& DiagnosticsEngine = Context->getDiagnostics();
            RewriterPointer Rewriter;
            if (DoRewrite) {
                Rewriter = createRewriter(DiagnosticsEngine, *Context);
            }

            auto FixIt = FixItHint::CreateReplacement(fun->getBeginLoc(), " olol ");
            auto& DE = Context->getDiagnostics();
            const auto ID = DE.getCustomDiagID(clang::DiagnosticsEngine::Remark, "Please rename this");
            DE.Report(fun->getBeginLoc(), ID).AddFixItHint(FixIt);

            if (DoRewrite) {
                assert(Rewriter != nullptr);

                llvm::raw_ostream os();

                //            Rewriter->WriteFixedFile(FullLocation.getFileID());
                Rewriter->WriteFixedFiles();
            }
        }

    }

}