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
#include "clang/Frontend/CompilerInvocation.h"


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

#include "Guesser.h"
#include "CommandLineOpts.h"
#include "ResourceTypes.h"
#include "clang/Frontend/CompilerInstance.h"
#include <clang/Sema/Sema.h>
#include "AutoConcept.h"
#include "FrontendAction.h"

namespace auto_concept {

    /// A custom \c FrontendActionFactory so that we can pass the options to the constructor of the tool.
    class ToolFactory : public clang::tooling::FrontendActionFactory {
        using MatchFinder = clang::ast_matchers::MatchFinder;
        using MatchFinder = clang::ast_matchers::MatchFinder;

        AutoConceptTuState tuState = AutoConceptTuState::ActingOnResults;
        int fileIndex = -1;
        GuesserCollection& guesserCollection;
    public:
        using MatchFinder = clang::ast_matchers::MatchFinder;
        std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher;
        std::function<void(const MatchFinder::MatchResult&)> customMatchHandler;
        std::shared_ptr<Resources> resources;
        std::string injectionProbingSuffix = "AutoConceptTempFile";
        AutoConceptGlobalState globalState = AutoConceptGlobalState::FinalPass;

        std::unique_ptr<clang::FrontendAction> create() override;

        ToolFactory(GuesserCollection& guesserCollection) :guesserCollection{ guesserCollection } { }

        bool runInvocation(
            std::shared_ptr<clang::CompilerInvocation> Invocation, clang::FileManager* Files,
            std::shared_ptr<clang::PCHContainerOperations> PCHContainerOps,
            clang::DiagnosticConsumer* DiagConsumer) override;

    };
}