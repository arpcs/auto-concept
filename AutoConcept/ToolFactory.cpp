
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

#include "ToolFactory.h"

namespace auto_concept {
    using namespace clang;

    std::unique_ptr<clang::FrontendAction> ToolFactory::create() {
        if (tuState == AutoConceptTuState::InjectingProbes) {
            return std::make_unique<auto_concept::Action>(true, injectionProbingSuffix, customMatcher, customMatchHandler, resources, tuState, guesserCollection.get(fileIndex));
        }
        return std::make_unique<auto_concept::Action>(CLOptions::RewriteOption, CLOptions::RewriteSuffixOption, customMatcher, customMatchHandler, resources, tuState, guesserCollection.get(fileIndex));
    }

    bool ToolFactory::runInvocation(
        std::shared_ptr<CompilerInvocation> Invocation, FileManager* Files,
        std::shared_ptr<PCHContainerOperations> PCHContainerOps,
        DiagnosticConsumer* DiagConsumer) {

        // FixMe: a less crude solution
        if (globalState == AutoConceptGlobalState::FinalPass || CLOptions::SkipProbingOption) tuState = AutoConceptTuState::ActingOnResults;
        if (globalState == AutoConceptGlobalState::InjectionPass) {
            tuState = AutoConceptTuState::InjectingProbes;
            if (Invocation && Invocation->getAnalyzerOpts() && !CLOptions::SkipProbingOption) {
                const auto cmds = Invocation->getAnalyzerOpts()->FullCompilerInvocation;
                if (cmds.find("." + injectionProbingSuffix + ".") != std::string::npos) {
                    tuState = AutoConceptTuState::CollectingResults;
                }
            }
        }
        if (CLOptions::SkipProbingOption || tuState == AutoConceptTuState::InjectingProbes || globalState == AutoConceptGlobalState::FinalPass) fileIndex++;

        // Create a compiler instance to handle the actual work.
        CompilerInstance Compiler(std::move(PCHContainerOps));
        Compiler.setInvocation(std::move(Invocation));
        Compiler.setFileManager(Files);

        // The FrontendAction can have lifetime requirements for Compiler or its members, and we need to ensure it's deleted earlier than Compiler.
        // So we pass it to an std::unique_ptr declared after the Compiler variable.
        std::unique_ptr<FrontendAction> ScopedToolAction(create());

        // Create the compiler's actual diagnostics engine.
        Compiler.createDiagnostics(DiagConsumer, /*ShouldOwnClient=*/false);
        if (!Compiler.hasDiagnostics())
            return false;

        Compiler.getDiagnostics().Clear();
        if (tuState == AutoConceptTuState::CollectingResults && CLOptions::LogLevelOption < 2) Compiler.getDiagnostics().setSuppressAllDiagnostics(true);
        else Compiler.getDiagnostics().setSuppressAllDiagnostics(false);

        Compiler.createSourceManager(*Files);

        const bool Success = Compiler.ExecuteAction(*ScopedToolAction);

        Files->clearStatCache();
        return Success;
    }
}
