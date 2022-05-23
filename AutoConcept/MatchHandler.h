#pragma once

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
#include <unordered_map>
#include <iostream>
#include <concepts>
#include <functional>

#include "CommandLine.h"
#include "ResourceTypes.h"
#include "AutoConcept.h"
#include "Guesser.h"

namespace auto_concept {
    class Consumer;
    class MatchHandler : public clang::ast_matchers::MatchFinder::MatchCallback {
        using MatchFinder = clang::ast_matchers::MatchFinder;
        using MatchResult = MatchFinder::MatchResult;
        using RewriterPointer = std::unique_ptr<clang::FixItRewriter>;
        FixItRewriterOptions FixItOptions;
        std::unordered_map< int64_t, clang::SmallVector<MatchResult> > matches;
        bool DoRewrite;
        std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher;
        std::function<void(const MatchFinder::MatchResult&)> customMatchHandler;
        std::shared_ptr<Resources> resources;
        AutoConceptTuState tuState;
        GuesserCollection::InnerType guessers;

        friend Consumer;
        /// Allocates a \c FixItRewriter and sets it as the client of the given \p DiagnosticsEngine.
        /// The \p Context is forwarded to the constructor of the \c FixItRewriter.
        RewriterPointer createRewriter(clang::DiagnosticsEngine& DiagnosticsEngine, clang::ASTContext& Context);
    public:
        /// \p DoRewrite and \p RewriteSuffix are the command line options passed to the tool.
        MatchHandler(bool DoRewrite, const std::string& RewriteSuffix, 
            std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher,
            std::function<void(const MatchFinder::MatchResult&)> customMatchHandler,
            std::shared_ptr<Resources> resources,
            AutoConceptTuState tuState,
            GuesserCollection::InnerType guessers)
            : FixItOptions(RewriteSuffix), DoRewrite(DoRewrite), customMatcher(customMatcher), customMatchHandler(customMatchHandler),
            resources{ resources }, tuState{ tuState }, guessers{ guessers } {}

        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& Result) final;
        virtual void onStartOfTranslationUnit() final;
        virtual void onEndOfTranslationUnit() final;

    };

}