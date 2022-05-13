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
#include "llvm/Support/WithColor.h"

// Standard includes
#include <cassert>
#include <memory>
#include <string>
#include <type_traits>
#include <filesystem>
#include <iostream>
#include <concepts>

#include "AutoConcept.h"
#include "Matchers.h"
#include "MatchHandler.h"
#include "Consumer.h"
#include "CommandLine.h"
#include "FrontendAction.h"

namespace auto_concept {

    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;
    using namespace llvm::cl;


    int RunApp(int argc, const char** argv) {
        auto ExpectedParser = CommonOptionsParser::create(argc, argv, CLOptions::MyToolCategory);
        if (!ExpectedParser) {
            // Fail gracefully for unsupported options.
            llvm::WithColor color(llvm::errs(), raw_ostream::Colors::RED);
            llvm::errs() << ExpectedParser.takeError();
            return 1;
        }

        CommonOptionsParser& OptionsParser = ExpectedParser.get();
        ClangTool    Tool(OptionsParser.getCompilations(),
            OptionsParser.getSourcePathList(),
            std::make_shared<PCHContainerOperations>(),
            llvm::vfs::getRealFileSystem()
        );

        auto factory = std::make_unique<ToolFactory>();

        return Tool.run(factory.get());
    }

    int RunAppTest(std::string& virtualFile) {
        // Prepare for testing
        int argc = 5;
        const std::string testFileIn = "virtualTestFile.cpp";
        const std::string testSuffix = "test.cpp";
        const std::string testFileOut = testFileIn + testSuffix;
        const std::string suffixRewriteArg = "-rewrite-suffix=" + testSuffix;
        const char* argv[] = { "AutoConceptTest", testFileIn.c_str(),"-rewrite",suffixRewriteArg.c_str(),"--" };

        if (std::filesystem::exists(testFileOut)) std::filesystem::remove(testFileOut);

        auto ExpectedParser = CommonOptionsParser::create(argc, argv, CLOptions::MyToolCategory);
        if (!ExpectedParser) {
            // Fail gracefully for unsupported options.
            llvm::WithColor color(llvm::errs(), raw_ostream::Colors::RED);
            llvm::errs() << ExpectedParser.takeError();
            return 1;
        }
        CommonOptionsParser& OptionsParser = ExpectedParser.get();
        //StandaloneToolExecutor
        //llvm::vfs::InMemoryFileSystem s();
        //IntrusiveRefCntPtr<llvm::vfs::FileSystem>(s);
        //llvm::vfs::InMemoryFileSystem fs();
        //auto hmi = IntrusiveRefCntPtr<llvm::vfs::FileSystem>(std::make_unique<llvm::vfs::InMemoryFileSystem>());

        ClangTool    Tool(OptionsParser.getCompilations(),
            OptionsParser.getSourcePathList(),
            std::make_shared<PCHContainerOperations>(),
            //hmi
            llvm::vfs::getRealFileSystem()
            //IntrusiveRefCntPtr<llvm::vfs::FileSystem>(  std::make_unique<llvm::vfs::InMemoryFileSystem>() )
            //IntrusiveRefCntPtr(llvm::vfs::InMemoryFileSystem())
        );

        // Map the string reference to a virtual file when testing
        Tool.mapVirtualFile(testFileIn, virtualFile);
      

        //auto action = newFrontendActionFactory<AutoConcept::Action>();
        auto factory = std::make_unique<ToolFactory>();
        auto result = Tool.run(factory.get());

        // Todo: a less ugly version
        if (auto FixedVirtualFile = Tool.getFiles().getVirtualFileSystem().openFileForRead(testFileOut)) {
            if (auto FixedVirtualFileBuffer = FixedVirtualFile.get().get()->getBuffer(testFileOut)) {
                virtualFile = FixedVirtualFileBuffer.get().get()->getBuffer().str();
            }
        }

        return result;
    }

}