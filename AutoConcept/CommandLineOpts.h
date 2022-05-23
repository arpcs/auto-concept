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
#include <ranges>
#include <iostream>
#include <concepts>

namespace auto_concept {

    struct CLOptions {

        // Apply a custom category to all command-line options so that they are the only ones displayed.
        static llvm::cl::OptionCategory MyToolCategory;

        // CommonOptionsParser declares HelpMessage with a description of the common
        // command-line options related to the compilation database and input files.
        // It's nice to have this help message in all tools.
        static llvm::cl::extrahelp CommonHelp;

        static llvm::cl::opt<bool> RewriteOption;
        static llvm::cl::opt<std::string> RewriteSuffixOption;
        static llvm::cl::opt<bool> SkipProbingOption;
        static llvm::cl::opt<bool> GenerateResourcesOption;
        static llvm::cl::list<std::string> TestConceptOption;
        
        static llvm::cl::list<std::string> IgnoreTypeOption;
        static llvm::cl::opt<int> MaxPreventOption;
        static llvm::cl::opt<int> MaxAllowOption;
        static llvm::cl::opt<int> LogLevelOption;
        static llvm::cl::opt<bool> KeepTempFilesOption;
        //static llvm::cl::opt<int> MinPreventOption;

        static llvm::cl::alias RewriteOptionAlias;
        static llvm::cl::alias RewriteSuffixOptionAlias;
        static llvm::cl::alias SkipProbingOptionAlias;
        //static llvm::cl::alias ThresholdOptionAlias;

        // A help message for this specific tool can be added afterwards.
        static llvm::cl::extrahelp MoreHelp;

        template<typename T, typename ValT>
        static bool contains(T& optionList, ValT& val) {
            return std::find(optionList.begin(), optionList.end(), val) != optionList.end();
        }
    };


    class FixItRewriterOptions : public clang::FixItOptions {
    public:
        using super = clang::FixItOptions;

        /// Constructor.
        ///
        /// The \p RewriteSuffix is the option from the command line.
        explicit FixItRewriterOptions(const std::string& RewriteSuffix) : RewriteSuffix(RewriteSuffix) {
            super::InPlace = false;
        }

        /// For a file to be rewritten, returns the (possibly) new filename.
        ///
        /// If the \c RewriteSuffix is empty, returns the \p Filename, causing
        /// in-place rewriting. If it is not empty, the \p Filename with that suffix
        /// is returned.
        std::string RewriteFilename(const std::string& Filename, int& fd) override;

    private:
        /// The suffix appended to rewritten files.
        std::string RewriteSuffix;
    };

}