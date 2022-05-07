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

#include "Defines.h"
#include "AutoConcept.h"
#include "TemplateInvariants.h"

namespace AutoConcept{

    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;

    class FixItRewriterOptions : public clang::FixItOptions {
    public:
        using super = clang::FixItOptions;

        /// Constructor.
        ///
        /// The \p RewriteSuffix is the option from the command line.
        explicit FixItRewriterOptions(const std::string& RewriteSuffix)
            : RewriteSuffix(RewriteSuffix) {
            super::InPlace = false;
            //super::InPlace = true;
        }

        /// For a file to be rewritten, returns the (possibly) new filename.
        ///
        /// If the \c RewriteSuffix is empty, returns the \p Filename, causing
        /// in-place rewriting. If it is not empty, the \p Filename with that suffix
        /// is returned.
        std::string RewriteFilename(const std::string& Filename, int& fd) override {
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

    private:
        /// The suffix appended to rewritten files.
        std::string RewriteSuffix;
    };

    class MatchHandler : public MatchFinder::MatchCallback {
        using MatchResult = clang::ast_matchers::MatchFinder::MatchResult;
        using RewriterPointer = std::unique_ptr<clang::FixItRewriter>;
        FixItRewriterOptions FixItOptions;
        bool DoRewrite;
        /// Allocates a \c FixItRewriter and sets it as the client of the given \p
        /// DiagnosticsEngine.
        ///
        /// The \p Context is forwarded to the constructor of the \c FixItRewriter.
        RewriterPointer createRewriter(clang::DiagnosticsEngine& DiagnosticsEngine,
            clang::ASTContext& Context) {
            auto Rewriter =
                std::make_unique<clang::FixItRewriter>(DiagnosticsEngine,
                    Context.getSourceManager(),
                    Context.getLangOpts(),
                    &FixItOptions);

            // Note: it would make more sense to just create a raw pointer and have the
            // DiagnosticEngine own it. However, the FixItRewriter stores a pointer to
            // the client of the DiagnosticsEngine when it gets constructed with it.
            // If we then set the rewriter to be the client of the engine, the old
            // client gets destroyed, leading to happy segfaults when the rewriter
            // handles a diagnostic.
            DiagnosticsEngine.setClient(Rewriter.get(), /*ShouldOwnClient=*/false);

            return Rewriter;
        }
    public:
        /// Constructor.
        ///
        /// \p DoRewrite and \p RewriteSuffix are the command line options passed
        /// to the tool.
        std::string deletethis;
        MatchHandler(bool DoRewrite, const std::string& RewriteSuffix)
            : FixItOptions(RewriteSuffix), DoRewrite(DoRewrite) {
            deletethis = RewriteSuffix;
        }
        virtual void run(const MatchFinder::MatchResult& Result) {
            ASTContext* Context = Result.Context;
            auto& map = Result.Nodes.getMap();

            

            for( auto& m: map){
                llvm::outs() << "Match found: " << m.first << "\n";
                llvm::outs() << "rewriting=" << DoRewrite << "\tsuffix=" << deletethis << "\n";
                m.second.dump(llvm::outs(), *Context);
            }

            if (const auto* fun = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("function-to-autoconcept")) {
                const auto& body = fun->getAsFunction()->getBody();
                llvm::outs() << "body pointer: " << body << "\n";
                FullSourceLoc FullLocation = Context->getFullLoc(fun->getBeginLoc());
                llvm::outs() << "Found function-to-autoconcept at " << FullLocation.getSpellingLineNumber() << ":" 
                    << FullLocation.getSpellingColumnNumber() << "\n";

                auto& DiagnosticsEngine = Context->getDiagnostics();
                RewriterPointer Rewriter;
                if (DoRewrite) {
                    Rewriter = createRewriter(DiagnosticsEngine, *Context);
                }

                auto FixIt = FixItHint::CreateReplacement(fun->getBeginLoc(), " olol ");
                auto& DE = Context->getDiagnostics();
                const auto ID = DE.getCustomDiagID(clang::DiagnosticsEngine::Remark, "Please rename this");
                DE.Report(fun->getBeginLoc(), ID).AddFixItHint(FixIt);

                if (DoRewrite) {
                    assert(Rewriter != nullptr);
                    
                    llvm::raw_ostream os();
        
        //            Rewriter->WriteFixedFile(FullLocation.getFileID());
                    Rewriter->WriteFixedFiles();
                }
             }

        }
    };

    class Consumer : public clang::ASTConsumer {
        /// Our callback for matches.
        MatchHandler handler;

        /// The MatchFinder we use for matching on the AST.
        clang::ast_matchers::MatchFinder matchFinder;
    public:
        /// Constructor.
        ///
        /// All arguments are forwarded to the \c MatchHandler.
        template <typename... Args>
        explicit Consumer(Args&&... args) : handler(std::forward<Args>(args)...) {
        //void HandleTranslationUnit(clang::ASTContext& Context) {
            using namespace clang::ast_matchers;

            /*const auto Matcher = functionTemplateDecl(
                isExpansionInMainFile(),
                has(
                    functionDecl(
                        hasAnyBody(
                            compoundStmt(
                                hasDescendant(
                                    callExpr( 
                                        //has(unresolvedLookupExpr( has(functionDecl()) ) )

                                        callee(functionDecl(
                                            hasName("advance"),
                                            hasParameter(0,varDecl().bind("advance-param"))
                                        ))

                                    ).bind("advance-function")
                                )
                            )
                        )
                    )                    
                )            
            ).bind("function-to-autoconcept");*/

            const auto matcher = functionTemplateDecl(
                isExpansionInMainFile(),
                has(
                    functionDecl()
                )
            ).bind("function-to-autoconcept");

            std::vector<int>::iterator;

            std::string dynMatcher = R"(functionTemplateDecl(
                isExpansionInMainFile(),
                has(
                    functionDecl()
                )
            ).bind("asdLOl"))";

            using namespace llvm;
            using namespace clang::ast_matchers::dynamic;
            Diagnostics Diag;
            dynamic::VariantValue Value;
            llvm::StringRef dynMatcherRef = dynMatcher;
            auto parsedMatcher = Parser::parseMatcherExpression(dynMatcherRef, &Diag);
            if (parsedMatcher.hasValue()) {
                auto& parsedMatcherVal = parsedMatcher.getValue();
                matchFinder.addDynamicMatcher(parsedMatcherVal, &handler);
            }

            //DynTypedMatcher matcher = dynMatcher;
            //llvm::Optional<DynTypedMatcher> M = matcher.tryBind("root");
            //auto MaybeBoundMatcher = *M;
            //matchFinder.addDynamicMatcher(rett, &handler);
            //matchFinder.addDynamicMatcher(MaybeBoundMatcher, &handler);


            auto matchers = GetTemplateInvariants();
            for (const auto& matcher: matchers)
            {
                matchFinder.addMatcher(matcher, &handler);
            }
            //matchFinder.addMatcher(matcher, &handler);

            //matchFinder.addMatcher(matcher, &handler);
            //Finder.matchAST(Context);
        }
        void HandleTranslationUnit(clang::ASTContext& Context) override {
            matchFinder.matchAST(Context);
        }
    };

    /// Creates an `ASTConsumer` that defines the matcher.
    /*class Action : public clang::ASTFrontendAction {
    public:
        using ASTConsumerPointer = std::unique_ptr<clang::ASTConsumer>;

        ASTConsumerPointer
            CreateASTConsumer(clang::CompilerInstance&, llvm::StringRef) override {
            return std::make_unique<Consumer>();
        }
    };*/
    class Action : public clang::ASTFrontendAction {
    public:
        using ASTConsumerPointer = std::unique_ptr<clang::ASTConsumer>;

        /// Constructor, taking the \p RewriteOption and \p RewriteSuffixOption.
        Action(bool DoRewrite, const std::string& RewriteSuffix)
            : DoRewrite(DoRewrite), RewriteSuffix(RewriteSuffix) {
        }

        /// Creates the Consumer instance, forwarding the command line options.
        ASTConsumerPointer CreateASTConsumer(clang::CompilerInstance& Compiler,
            llvm::StringRef Filename) override {
            return std::make_unique<Consumer>(DoRewrite, RewriteSuffix);
        }

    private:
        /// Whether we want to rewrite files. Forwarded to the consumer.
        bool DoRewrite;

        /// The suffix for rewritten files. Forwarded to the consumer.
        std::string RewriteSuffix;
    };
}

namespace {
    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm::cl;
    using namespace AutoConcept;

    // Apply a custom category to all command-line options so that they are the
    // only ones displayed.
    static OptionCategory MyToolCategory("my-tool options");

    // CommonOptionsParser declares HelpMessage with a description of the common
    // command-line options related to the compilation database and input files.
    // It's nice to have this help message in all tools.
    static extrahelp CommonHelp(CommonOptionsParser::HelpMessage);


    llvm::cl::opt<bool>
        RewriteOption("rewrite",
            llvm::cl::init(false),
            llvm::cl::desc("If set, emits rewritten source code"),
            llvm::cl::cat(MyToolCategory));

    llvm::cl::opt<std::string> RewriteSuffixOption(
        "rewrite-suffix",
        llvm::cl::desc("If -rewrite is set, changes will be rewritten to a file "
            "with the same name, but this suffix"),
        llvm::cl::cat(MyToolCategory));


    // A help message for this specific tool can be added afterwards.
    static extrahelp MoreHelp("\nMore help text...");
}

/// A custom \c FrontendActionFactory so that we can pass the options
/// to the constructor of the tool.
struct ToolFactory : public clang::tooling::FrontendActionFactory {
    std::unique_ptr<FrontendAction> create() override {
        return std::make_unique<AutoConcept::Action> (RewriteOption, RewriteSuffixOption);
    }
};

int RunApp(int argc, const char** argv) {
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!ExpectedParser) {
        // Fail gracefully for unsupported options.
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
    const char* argv[] = { "AutoConceptTest", testFileIn.c_str(),"-rewrite",suffixRewriteArg.c_str(),"--"};

    auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!ExpectedParser) {
        // Fail gracefully for unsupported options.
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
    auto resultStr = Tool.getFiles().getVirtualFileSystem().openFileForRead(testFileOut).get().get()->getBuffer(testFileOut).get().get()->getBuffer().str();
    virtualFile = resultStr;
    
    return result;
}
/*
int main(int argc, const char** argv) {
    return RunApp(argc, argv);
}*/