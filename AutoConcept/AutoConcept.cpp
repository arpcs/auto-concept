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
#include <functional>

#include "AutoConcept.h"
#include "Matchers.h"
#include "MatchHandler.h"
#include "Consumer.h"
#include "CommandLine.h"
#include "FrontendAction.h"
#include "ResourceTypes.h"
#include "BasicResourceGenerators.h"


namespace auto_concept {

    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;
    using namespace llvm::cl;


    int RunApp(int argc, const char** argv) {

       /* FILE* asdf, * asdf2;
       // auto pastErr = stderr;
        //auto pastOut = stdout;
        fflush(stdout);
        fflush(stderr);
        auto err1 = freopen_s(&asdf, "myfile.txt", "w", stdout);
        auto err2 = freopen_s(&asdf2, "myfile2.txt", "w", stderr);*/

       /* fclose(asdf);
        fclose(asdf2);
        freopen_s(&asdf, "CONOUT$", "w", stdout);
        freopen_s(&asdf2, "CONOUT$", "w", stderr);
        fflush(stdout);
        fflush(stderr);*/

        auto ExpectedParser = CommonOptionsParser::create(argc, argv, CLOptions::MyToolCategory);
        if (!ExpectedParser) {
            // Fail gracefully for unsupported options.
            llvm::WithColor color(llvm::errs(), raw_ostream::Colors::RED);
            llvm::errs() << ExpectedParser.takeError();
            return 1;
        }

 

        auto resources = std::make_shared<Resources>();

        CommonOptionsParser& OptionsParser = ExpectedParser.get();
        ClangTool    Tool(OptionsParser.getCompilations(),
            OptionsParser.getSourcePathList(),
            std::make_shared<PCHContainerOperations>(),
            llvm::vfs::getRealFileSystem()
        );

        auto factory = std::make_unique<ToolFactory>();
        factory.get()->resources = resources;
        return Tool.run(factory.get());
    }

    int RunAppOnVirtual(std::string& virtualFile, 
        std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher,
        std::function<void(const MatchFinder::MatchResult&)>     customMatchHandler) {

        /*FILE* asdf, * asdf2;
        auto pastErr = stderr;
        auto pastOut = stdout;
        fflush(stdout);
        fflush(stderr);
        //auto err1= freopen_s(&asdf, "myfile.txt", "w", stdout);
        auto err2 = freopen_s(&asdf2, "myfile2.txt", "w", stderr);*/
        /*fclose(asdf);
        fclose(asdf2);
        freopen_s(&asdf,"CONOUT$", "w", stdout);
        freopen_s(&asdf2,"CONOUT$", "w", stderr);
        fflush(stdout);
        fflush(stderr);*/

        auto resources = std::make_shared<Resources>();
        if (!customMatcher && !customMatchHandler) {
            //FillMissingResources(res);
        }
       // llvm::raw_null_ostream throwArayStdOut{};
        //auto savedStdOut = &outs();
        for (int pass = 0; pass < 2; pass++) {

           // if (pass == 0) outs().tie(&throwArayStdOut);
            //else outs().tie(savedStdOut);

            

            // Prepare for testing
            int argc = 7;
            const std::string virtualFileIn = "VirtualInFile.cpp";
            const std::string virtualFileIn2 = "VirtualInFile2.cpp";
            const std::string virtualSuffix = "VirtualOut";
            const std::string virtualFileOut = virtualFileIn + virtualSuffix;
            const std::string suffixRewriteArg = "-rewrite-suffix=" + virtualSuffix;
            const char* argv[] = { "AutoConceptTest", virtualFileIn.c_str(),"VirtualInFile.AutoConceptTempFile.cpp",virtualFileIn2.c_str(),"-rewrite",suffixRewriteArg.c_str(),"--extra-arg-before=-std=c++2b","--extra-arg=-Xclang","--extra-arg=-fcolor-diagnostics","--"};

            if (std::filesystem::exists(virtualFileOut)) std::filesystem::remove(virtualFileOut);

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

            


            // Map the string reference to a virtual file when testing
            Tool.mapVirtualFile(virtualFileIn, virtualFile);
            Tool.mapVirtualFile(virtualFileIn2, virtualFile);

            auto factory = std::make_unique<ToolFactory>();
            factory.get()->customMatcher = customMatcher;
            factory.get()->customMatchHandler = customMatchHandler;
            factory.get()->resources = resources;
            //factory.get()->injectingErrors = (pass == 0);
            
            auto result = Tool.run(factory.get());

            if (auto FixedVirtualFile = Tool.getFiles().getVirtualFileSystem().openFileForRead(virtualFileOut)) {
                if (auto FixedVirtualFileBuffer = FixedVirtualFile.get().get()->getBuffer(virtualFileOut)) {
                    virtualFile = FixedVirtualFileBuffer.get().get()->getBuffer().str();
                }
            }

            if (pass == 1) return result;
        }

        return -1;
    }

}