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
#include <vector>

#include "AutoConcept.h"
#include "Matchers.h"
#include "MatchHandler.h"
#include "Consumer.h"
#include "CommandLineOpts.h"
#include "FrontendAction.h"
#include "ResourceTypes.h"
#include "BasicResourceGenerators.h"
#include "Guesser.h"
#include "ToolFactory.h"


namespace auto_concept {

    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;
    using namespace llvm::cl;


    int RunApp(int argc, const char** argv) {

        auto resources = std::make_shared<Resources>();
        GuesserCollection guesserCollection;

        auto ExpectedParser = CommonOptionsParser::create(argc, argv, CLOptions::MyToolCategory);
        if (!ExpectedParser) {
            if (CLOptions::GenerateResourcesOption) {
                FillMissingResources(*resources.get());
                return 0;
            }
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

        auto factory = std::make_unique<ToolFactory>(guesserCollection);
        factory.get()->resources = resources;
        return Tool.run(factory.get());
    }

    int RunAppOnVirtual(std::string& virtualFile, const std::vector<std::string>& args,
        std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher,
        std::function<void(const MatchFinder::MatchResult&)>     customMatchHandler) {

        auto resources = std::make_shared<Resources>();
        GuesserCollection guesserCollection;
        for (int pass = 0; pass < 2; pass++) {

            // Prepare for testing
            std::string virtualFileIn = "VirtualInFile";
            const std::string virtualSuffix = "VirtualOut";
            const std::string virtualFileOut = virtualFileIn+ "." + virtualSuffix + ".cpp";
            virtualFileIn += ".cpp";
            const std::string suffixRewriteArg = "-rewrite-suffix=" + virtualSuffix;
            const std::string injectionSuffix = "AutoConceptTempFile";
            std::vector<const char*> arguments = { "AutoConceptTest", virtualFileIn.c_str(),"-rewrite",suffixRewriteArg.c_str(),"--extra-arg-before=-std=c++2b","--extra-arg=-ferror-limit=0"};

            for(const auto& arg: args) arguments.push_back(arg.c_str());

            arguments.push_back("--");
            std::vector<std::string> injectedArgs;
            int argc = arguments.size();
            if (pass == 0 && !customMatcher && !customMatchHandler) {
                auto ExpectedParser = CommonOptionsParser::create(argc, arguments.data(), CLOptions::MyToolCategory);
                if (ExpectedParser && !CLOptions::SkipProbingOption) {
                    for (const auto& filename: ExpectedParser->getSourcePathList())
                    {
                        const std::string newFilename = filename.substr(0, filename.find_last_of('.') + 1) + injectionSuffix + filename.substr(filename.find_last_of('.'));
                        injectedArgs.push_back(newFilename);
                        for (auto it = arguments.begin(); it != arguments.end(); it++) {
                            if (std::string(*it) == filename) {
                                arguments.insert(it+1, injectedArgs.back().c_str());
                                break;
                            }
                        }
                        argc++;
                    }  
                }
            }

            if (std::filesystem::exists(virtualFileOut)) std::filesystem::remove(virtualFileOut);
            
            argc = arguments.size();
            auto ExpectedParser = CommonOptionsParser::create(argc, arguments.data(), CLOptions::MyToolCategory);
            
            if (!ExpectedParser) {
                // Fail gracefully for unsupported options.
                llvm::WithColor color(llvm::errs(), raw_ostream::Colors::RED);
                llvm::errs() << ExpectedParser.takeError();
                return 1;
            }


            for (const auto& filename : ExpectedParser->getSourcePathList())
            {
                const std::string newFilename = filename.substr(0, filename.find_last_of('.') + 1) + injectionSuffix + filename.substr(filename.find_last_of('.'));
                if (pass == 1 && std::filesystem::exists(newFilename) && !CLOptions::KeepTempFilesOption)
                    std::filesystem::remove(newFilename);
                if (pass == 0 && std::filesystem::exists(newFilename))
                    std::filesystem::remove(newFilename);
                argc++;
            }


            CommonOptionsParser& OptionsParser = ExpectedParser.get();
            ClangTool    Tool(OptionsParser.getCompilations(),
                OptionsParser.getSourcePathList(),
                std::make_shared<PCHContainerOperations>(),
                llvm::vfs::getRealFileSystem()
            );


            // Map the string reference to a virtual file when testing
            Tool.mapVirtualFile(virtualFileIn, virtualFile);

            auto factory = std::make_unique<ToolFactory>(guesserCollection);
            factory.get()->customMatcher = customMatcher;
            factory.get()->customMatchHandler = customMatchHandler;
            factory.get()->resources = resources;
            if (pass == 0 && !CLOptions::SkipProbingOption) factory.get()->globalState = AutoConceptGlobalState::InjectionPass;
            else                                            factory.get()->globalState = AutoConceptGlobalState::FinalPass;
            factory.get()->injectionProbingSuffix = injectionSuffix;
            
            auto result = Tool.run(factory.get());

            if (pass == 1 || CLOptions::SkipProbingOption || customMatcher || customMatchHandler) {
                if (auto FixedVirtualFile = Tool.getFiles().getVirtualFileSystem().openFileForRead(virtualFileOut)) {
                    if (auto FixedVirtualFileBuffer = FixedVirtualFile.get().get()->getBuffer(virtualFileOut)) {
                        virtualFile = FixedVirtualFileBuffer.get().get()->getBuffer().str();
                    }
                }
                if (!CLOptions::KeepTempFilesOption && std::filesystem::exists(virtualFileOut)) std::filesystem::remove(virtualFileOut);
                return result;
            }
        }

        return -1;
    }

}