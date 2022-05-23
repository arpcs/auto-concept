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
        OptionCategory CLOptions::MyToolCategory("AutoConcept options");

        // CommonOptionsParser declares HelpMessage with a description of the common
        // command-line options related to the compilation database and input files.
        // It's nice to have this help message in all tools.
        extrahelp CLOptions::CommonHelp(CommonOptionsParser::HelpMessage);

        llvm::cl::opt<bool> CLOptions::RewriteOption("rewrite",
            llvm::cl::init(false),
            llvm::cl::desc("If set, emits rewritten source code"),
            llvm::cl::cat(MyToolCategory));
        llvm::cl::opt<bool> CLOptions::SkipProbingOption("skip-probing",
            llvm::cl::init(false),
            llvm::cl::desc("If set, skips probing templates for valid arguments"),
            llvm::cl::cat(MyToolCategory));
        llvm::cl::opt<std::string> CLOptions::RewriteSuffixOption("rewrite-suffix",
            llvm::cl::desc("If -rewrite is set, changes will be rewritten to a file with the same name, but this suffix"),
            llvm::cl::cat(MyToolCategory));

        llvm::cl::opt<bool> CLOptions::GenerateResourcesOption("generate-resources",
            llvm::cl::init(false),
            llvm::cl::desc("If set, the program will generate new resources files, call this whenever Types.txt or Concepts.txt was changed"),
            llvm::cl::cat(MyToolCategory));

        llvm::cl::opt<int> CLOptions::LogLevelOption("log-level",
            llvm::cl::init(0),
            llvm::cl::desc("Logging level from 0 to 3, the lower the value the less details the program gives. ( 3 reports injected errors )"),
            llvm::cl::cat(MyToolCategory));

        llvm::cl::opt<bool> CLOptions::KeepTempFilesOption("keep-temp-files",
            llvm::cl::init(false),
            llvm::cl::desc("If set, the program won't delete the generated temp files"),
            llvm::cl::cat(MyToolCategory));

        llvm::cl::list<std::string> CLOptions::TestConceptOption("test-concept",
            llvm::cl::NumOccurrencesFlag(llvm::cl::ZeroOrMore),
            llvm::cl::desc("Print why the given concept wasn't considered"),
            llvm::cl::cat(MyToolCategory));
        llvm::cl::list<std::string> CLOptions::IgnoreTypeOption("ignore-type",
            llvm::cl::NumOccurrencesFlag(llvm::cl::ZeroOrMore),
            llvm::cl::desc("Ignores the following type when choosing concepts"),
            llvm::cl::cat(MyToolCategory));
        /*llvm::cl::opt<int> CLOptions::MinPreventOption("min-prevent",
            llvm::cl::init(0),
            llvm::cl::desc("Min number of specialization types the genereted concept should disallow"),
            llvm::cl::cat(MyToolCategory));*/
        llvm::cl::opt<int> CLOptions::MaxPreventOption("max-prevent",
            llvm::cl::init(0),
            llvm::cl::desc("Max number of semantically correct specializations the genereted concept is allowed to prevent"),
            llvm::cl::cat(MyToolCategory));
        llvm::cl::opt<int> CLOptions::MaxAllowOption("max-allow",
            llvm::cl::init(0),
            llvm::cl::desc("Max number of semantically incorrect specializations the genereted concept could allow"),
            llvm::cl::cat(MyToolCategory));

        llvm::cl::alias CLOptions::RewriteSuffixOptionAlias("x",
            llvm::cl::desc("Alias for rewrite-suffix option"),
            llvm::cl::aliasopt(CLOptions::RewriteSuffixOption)
        ); 
        llvm::cl::alias CLOptions::SkipProbingOptionAlias("s",
            llvm::cl::desc("Alias for skip-probing option"),
            llvm::cl::aliasopt(CLOptions::SkipProbingOption)
        );
        llvm::cl::alias CLOptions::RewriteOptionAlias("r",
            llvm::cl::desc("Alias for rewrite option"),
            llvm::cl::aliasopt(CLOptions::RewriteOption)
        );

        // A help message for this specific tool can be added afterwards.
        extrahelp CLOptions::MoreHelp("\nMore help text...");

        std::string FixItRewriterOptions::RewriteFilename(const std::string& Filename, int& fd) {
            fd = -1;

            if (CLOptions::LogLevelOption > 0) llvm::errs() << "Rewriting FixIts ";

            if (RewriteSuffix.empty()) {
                if (CLOptions::LogLevelOption > 0) llvm::errs() << "in-place\n";
                return Filename;
            }

            const auto NewFilename = Filename.substr(0, Filename.find_last_of('.') + 1) + RewriteSuffix + Filename.substr(Filename.find_last_of('.'));
            if (CLOptions::LogLevelOption > 0) llvm::errs() << "from " << Filename << " to " << NewFilename << "\n";

            return NewFilename;
        }

}