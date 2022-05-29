
// Clang includes
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Basic/SourceLocation.h"
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
#include "llvm/Support/CommandLine.h"

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

// Own includes
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
#include "ConsoleHelpers.h"


namespace auto_concept {

    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;
    using namespace llvm::cl;
    using namespace std;

    // Running app on console arguments
    int RunApp(int argc, const char** argv) {

        // Collect argv to a vector
        vector<string> args;
        for (size_t i = 0; i < argc; i++) args.push_back(argv[i]);
        string empty;

        // Run the general case
        return RunApp(empty, args);
    }

    // Running app on general inputs
    int RunApp(std::string& virtualFile, 
        const std::vector<std::string> args,
        std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher,
        std::function<void(const MatchFinder::MatchResult&)>     customMatchHandler) {

        // Do we run on a source compiled from a string?
        bool virtualRun = virtualFile != "";

        // Do we skip probing?
        bool skipProbing = customMatcher || customMatchHandler;

        // Initializing basic resources for calculating concepts
        auto resources = std::make_shared<Resources>();
        GuesserCollection guesserCollection;

        // List of injected files to delete later
        vector<string> injectedTempFiles;

        // Program return value
        int success = 0;

        // We usually need 2 passes on the input files
        for (int pass = 0; pass < 2; pass++) {

            // Injecting extra arguments needed to us
            vector<string> argsCopy = args;
            argsCopy.push_back("--extra-arg-before=-std=c++2b");
            argsCopy.push_back("--extra-arg=-ferror-limit=0");
            if (virtualRun) {
                argsCopy.insert(argsCopy.begin(), "AutoConceptTest");
                argsCopy.push_back(Constants::virtualFileIn);

                // This will make the parser stop complaining about the missing compilation database
                argsCopy.push_back("--");
            }
            std::vector<const char*> finalArguments;

            // Placing -- to the right position
            if (auto minMinPos = find(argsCopy.begin(), argsCopy.end(), "--"); 
                minMinPos != argsCopy.end()) {
                iter_swap(minMinPos, argsCopy.end() - 1);
            }

            for(const auto& arg: argsCopy) finalArguments.push_back(arg.c_str());

            // Parse the given arguments
            int argc = finalArguments.size();
            auto ExpectedParser = CommonOptionsParser::create(argc, finalArguments.data(), CLOptions::MyToolCategory);

            // Update skip probing
            skipProbing |= CLOptions::SkipProbingOption;

            if (CLOptions::GenerateResourcesOption) {
                auto resourceVal = *resources.get();
                FillMissingResources(resourceVal);
                return 0;
            }
            
            // Fail if we have unsupported options
            if (!ExpectedParser) {
                llvm::WithColor color(llvm::errs(), raw_ostream::Colors::RED);
                llvm::errs() << ExpectedParser.takeError();
                return 1;
            }
            CommonOptionsParser& OptionsParser = ExpectedParser.get();

            // Change rewriting command line options when we have a virtual input
            if (virtualRun) {
                CLOptions::RewriteOption.setValue(true);
                CLOptions::RewriteSuffixOption.setValue(Constants::virtualSuffix);
            }

            // Getting source files
            vector<string> sourcePathList = OptionsParser.getSourcePathList();

            // Getting probe injected source files
            if (!skipProbing && pass==0)  sourcePathList = GetWithProbeFiles(sourcePathList, Constants::injectionSuffix, injectedTempFiles);


            // Creating the Clang Tool
            ClangTool    Tool(OptionsParser.getCompilations(),
                sourcePathList,
                std::make_shared<PCHContainerOperations>(),
                llvm::vfs::getRealFileSystem()
            );
            
            // Map the string reference to a virtual file if needed
            if (virtualRun) Tool.mapVirtualFile(Constants::virtualFileIn, virtualFile);

            // Setting up the Frontend Action Factory
            auto factory = std::make_unique<ToolFactory>(guesserCollection);
            factory.get()->customMatcher = customMatcher;
            factory.get()->customMatchHandler = customMatchHandler;
            factory.get()->resources = resources;
            if (pass == 0 && !skipProbing) factory.get()->globalState = AutoConceptGlobalState::InjectionPass;
            else                           factory.get()->globalState = AutoConceptGlobalState::FinalPass;
            factory.get()->injectionProbingSuffix = Constants::injectionSuffix;
            
            // Deleting temp files, because the rewriter on suffixed files wont work if the file already exist
            DeleteTempFiles(0, pass, injectedTempFiles);


            // Finally running the tool
            success |= Tool.run(factory.get());


            // Collecting virtual results and early exit
            if (pass == 1 || skipProbing)  {
                if (virtualRun) {
                    if (auto FixedVirtualFile = Tool.getFiles().getVirtualFileSystem().openFileForRead(Constants::virtualFileOut)) {
                        if (auto FixedVirtualFileBuffer = FixedVirtualFile.get().get()->getBuffer(Constants::virtualFileOut)) {
                            virtualFile = FixedVirtualFileBuffer.get().get()->getBuffer().str();
                        }
                    }
                }
                break;
            }

        }

        // Deleting temp files if the KeepTempFiles option flag wasn't flipped
        DeleteTempFiles(1, 2, injectedTempFiles);

        return success;
    }

    // Function for deleting temp files and injected probes
    void DeleteTempFiles(bool afterTollRun, int pass, const std::vector<std::string>& injectedFiles) {

        // Deleting virtual temp file
        if (!afterTollRun || !CLOptions::KeepTempFilesOption)
            if (std::filesystem::exists(Constants::virtualFileOut)) std::filesystem::remove(Constants::virtualFileOut);

        // Deleting probes
        for (const auto& filename : injectedFiles)
        {
            if (!std::filesystem::exists(filename)) continue;
            if (pass == 0) std::filesystem::remove(filename);
            if (afterTollRun == true && !CLOptions::KeepTempFilesOption) std::filesystem::remove(filename);
        }

    }

}

