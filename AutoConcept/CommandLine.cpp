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


namespace auto_concept {
    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;
    using namespace llvm::cl;


        // Apply a custom category to all command-line options so that they are the only ones displayed.
        OptionCategory CLOptions::MyToolCategory("my-tool options");

        // CommonOptionsParser declares HelpMessage with a description of the common
        // command-line options related to the compilation database and input files.
        // It's nice to have this help message in all tools.
        extrahelp CLOptions::CommonHelp(CommonOptionsParser::HelpMessage);

        llvm::cl::opt<bool> CLOptions::RewriteOption("rewrite",
                llvm::cl::init(false),
                llvm::cl::desc("If set, emits rewritten source code"),
                llvm::cl::cat(MyToolCategory));

        llvm::cl::opt<std::string> CLOptions::RewriteSuffixOption("rewrite-suffix",
            llvm::cl::desc("If -rewrite is set, changes will be rewritten to a file with the same name, but this suffix"),
            llvm::cl::cat(MyToolCategory));

        // A help message for this specific tool can be added afterwards.
        extrahelp CLOptions::MoreHelp("\nMore help text...");

        std::string FixItRewriterOptions::RewriteFilename(const std::string& Filename, int& fd) {
            fd = -1;

            llvm::errs() << "Rewriting FixIts ";

            if (RewriteSuffix.empty()) {
                llvm::errs() << "in-place\n";
                return Filename;
            }

            const auto NewFilename = Filename + RewriteSuffix;
            llvm::errs() << "from " << Filename << " to " << NewFilename << "\n";

            return NewFilename;
        }

}