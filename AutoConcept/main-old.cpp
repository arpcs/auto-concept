void asdf() {}

#if 0
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "llvm/Support/CommandLine.h"
/*
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include "clang/Basic/Diagnostic.h"

// LLVM includes
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/StringSet.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>
*/

namespace AutoConcept {

    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;

    /*class MyPrinter : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult& Result) {
            ASTContext* Context = Result.Context;
            if (const CallExpr* E =
                Result.Nodes.getNodeAs<clang::CallExpr>("functions")) {
                FullSourceLoc FullLocation = Context->getFullLoc(E->getBeginLoc());
                if (FullLocation.isValid()) {
                    llvm::outs() << "Found call at " << FullLocation.getSpellingLineNumber()
                        << ":" << FullLocation.getSpellingColumnNumber() << "\n";
                }
            }
        }
    };*/
    class MatchHandler : public MatchFinder::MatchCallback {
    public:
        virtual void run(const MatchFinder::MatchResult& Result) {
            ASTContext* Context = Result.Context;
            auto& map = Result.Nodes.getMap();
            for (auto& m : map) {
                //auto& E = m.second.dump();

                llvm::outs() << "Match: " << m.first << "\n";
                m.second.dump(llvm::outs(), *Context);

                //Result.Nodes.getNodeAs<clang::CallExpr>("functions")) {
                /*FullSourceLoc FullLocation = Context->getFullLoc(E->getBeginLoc());
                //if (FullLocation.isValid()) {
                llvm::outs() << "Found function at " << FullLocation.getSpellingLineNumber()
                    << ":" << FullLocation.getSpellingColumnNumber() << "\n";*/
                    //}
            }
            if (const auto* fun =
                Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functions")) {
                const auto& body = fun->getAsFunction()->getBody();
                llvm::outs() << "body: " << fun->hasBody() << " " << body << "\n";

                //findAll(callExpr()); hasDescendant
                //Result.Nodes.getNodeAs<clang::CallExpr>("functions")) {
                FullSourceLoc FullLocation = Context->getFullLoc(fun->getBeginLoc());
                //if (FullLocation.isValid()) {
                llvm::outs() << "Found function at " << FullLocation.getSpellingLineNumber()
                    << ":" << FullLocation.getSpellingColumnNumber() << "\n";
            }
            /*if (const auto* E =
                Result.Nodes.getNodeAs<clang::Stmt>("functions")) {
                //Result.Nodes.getNodeAs<clang::CallExpr>("functions")) {
                FullSourceLoc FullLocation = Context->getFullLoc(E->getBeginLoc());
                //if (FullLocation.isValid()) {
                    llvm::outs() << "Found function at " << FullLocation.getSpellingLineNumber()
                        << ":" << FullLocation.getSpellingColumnNumber() << "\n";
                //}
            }*/
        }
    };

    /*class MatchHandler : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        using MatchResult = clang::ast_matchers::MatchFinder::MatchResult;

        void run(const MatchResult& Result) {
            const auto* Destructor =
                Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("destructor");

            const clang::CXXRecordDecl* Base = Destructor->getParent();

            // Insert the new name of the base class or return if we've seen it already.
            const std::string BaseName = Base->getQualifiedNameAsString();
            if (auto [_, Success] = BaseNames.insert(BaseName); !Success) {
                return;
            }

            const auto* Derived =
                Result.Nodes.getNodeAs<clang::CXXRecordDecl>("derived");

            clang::DiagnosticsEngine& Diagnostics = Result.Context->getDiagnostics();
            const char Message[] =
                "'%0' should have a virtual destructor because '%1' derives from it";
            const unsigned ID =
                Diagnostics.getCustomDiagID(clang::DiagnosticsEngine::Warning, Message);

            // We can even warn about missing virtual when the user forgot to declare
            // the destructor alltogether! In that case, the diagnostic should point to
            // the class declaration instead of the destructor declaration.
            auto Location = Destructor->isUserProvided() ? Destructor->getBeginLoc()
                : Base->getLocation();

            clang::DiagnosticBuilder Builder = Diagnostics.Report(Location, ID);
            Builder.AddString(BaseName);
            Builder.AddString(Derived->getQualifiedNameAsString());

            // If the destructor is user-provided, we also recommend a FixItHint.
            if (Destructor->isUserProvided()) {
                const clang::FixItHint FixIt =
                    clang::FixItHint::CreateInsertion(Location, "virtual ");
                Builder.AddFixItHint(FixIt);
            }
        }

    private:
        // A set of all base-class names seen so far, so we avoid duplicate warnings.
        llvm::StringSet<> BaseNames;
    };*/

    class Consumer : public clang::ASTConsumer {
    public:
        /// Creates the `ASTMatcher` to match destructors and dispatches it on the TU.
        void HandleTranslationUnit(clang::ASTContext& Context) {
            using namespace clang::ast_matchers;

            // Want to match all classes, that are derived from classe, that have a
            // destructor tha tis not virtual. This leaves nothing to be done in the
            // `MatchHandler` than emitting a diagnostics.

            // clang-format off
            /*const auto Matcher =
                cxxRecordDecl(
                    isExpansionInMainFile(),
                    isDerivedFrom(
                        cxxRecordDecl(
                            has(cxxDestructorDecl(
                                unless(isVirtual())
                            ).bind("destructor"))))
                ).bind("derived");*/
            const auto Matcher = functionTemplateDecl(
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
                                            hasParameter(0, varDecl().bind("param"))
                                        ))
                                    ).bind("advance")
                                )
                            )
                        )
                    )
                )
            ).bind("functions");
            /*cxxMethodDecl(
               // isExpansionInMainFile()
            ).bind("functions");*/
            // clang-format on

            MatchHandler Handler;
            MatchFinder Finder;
            Finder.addMatcher(Matcher, &Handler);
            Finder.matchAST(Context);
        }
    };

    /// Creates an `ASTConsumer` that defines the matcher.
    class Action : public clang::ASTFrontendAction {
    public:
        using ASTConsumerPointer = std::unique_ptr<clang::ASTConsumer>;

        ASTConsumerPointer
            CreateASTConsumer(clang::CompilerInstance&, llvm::StringRef) override {
            return std::make_unique<Consumer>();
        }
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

    // A help message for this specific tool can be added afterwards.
    static extrahelp MoreHelp("\nMore help text...");
}

int main(int argc, const char** argv) {

    auto ExpectedParser = CommonOptionsParser::create(argc, argv, MyToolCategory);
    if (!ExpectedParser) {
        // Fail gracefully for unsupported options.
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    CommonOptionsParser& OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),
        OptionsParser.getSourcePathList());

    /*
    MyPrinter Printer;
    MatchFinder Finder;

    StatementMatcher functionMatcher =
        callExpr(callee(functionDecl(hasName("fun")))).bind("functions");

    Finder.addMatcher(functionMatcher, &Printer);

    auto action = newFrontendActionFactory(&Finder);*/

    auto action = newFrontendActionFactory<AutoConcept::Action>();

    return Tool.run(action.get());
}
#endif