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

#include <iostream>
#include <concepts>
#include <functional>

#include "CommandLine.h"
#include "ResourceTypes.h"

namespace auto_concept {

    /// Creates an `ASTConsumer` that defines the matcher.
    class Action : public clang::ASTFrontendAction {
    public:
        using ASTConsumerPointer = std::unique_ptr<clang::ASTConsumer>;
        using MatchFinder = clang::ast_matchers::MatchFinder;

        /// Constructor, taking the \p RewriteOption and \p RewriteSuffixOption.
        Action(bool DoRewrite, const std::string& RewriteSuffix, 
            std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher,
            std::function<void(const MatchFinder::MatchResult&)> customMatchHandler,
            std::shared_ptr<Resources> resources)
            : DoRewrite(DoRewrite), RewriteSuffix(RewriteSuffix), customMatcher(customMatcher), customMatchHandler(customMatchHandler), resources{ resources } {
        }

        /// Creates the Consumer instance, forwarding the command line options.
        ASTConsumerPointer CreateASTConsumer(clang::CompilerInstance& Compiler,
            llvm::StringRef Filename) override {
            return std::make_unique<Consumer>(DoRewrite, RewriteSuffix, customMatcher, customMatchHandler, resources);
        }

    private:
        // Custom mather and handler for convenience
        std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher;
        std::function<void(const MatchFinder::MatchResult&)> customMatchHandler;
        std::shared_ptr<Resources> resources;

        /// Whether we want to rewrite files. Forwarded to the consumer.
        bool DoRewrite;

        /// The suffix for rewritten files. Forwarded to the consumer.
        std::string RewriteSuffix;
    };

    /// A custom \c FrontendActionFactory so that we can pass the options
    /// to the constructor of the tool.
    struct ToolFactory : public clang::tooling::FrontendActionFactory {
        using MatchFinder = clang::ast_matchers::MatchFinder;
        std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher;
        std::function<void(const MatchFinder::MatchResult&)> customMatchHandler;
        std::shared_ptr<Resources> resources;

        std::unique_ptr<clang::FrontendAction> create() override {
            return std::make_unique<auto_concept::Action>(CLOptions::RewriteOption, CLOptions::RewriteSuffixOption, customMatcher, customMatchHandler, resources);
        }
    };

}