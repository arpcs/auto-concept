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
#include "clang/AST/ASTImporter.h"

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
#include "MatchHandler.h"


#include "clang/Sema/DeclSpec.h"
#include "clang/Sema/Initialization.h"
#include "clang/Sema/Lookup.h"
#include "clang/Sema/Overload.h"
#include "clang/Sema/ParsedTemplate.h"
#include "clang/Sema/Scope.h"
#include "clang/Sema/SemaInternal.h"
#include "clang/Sema/Template.h"
#include "clang/Sema/TemplateDeduction.h"

namespace auto_concept {
    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;
    using namespace std;

    MatchHandler::RewriterPointer MatchHandler::createRewriter(clang::DiagnosticsEngine& DiagnosticsEngine,
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

    void MatchHandler::run(const MatchFinder::MatchResult& Result) {

        if (this->customMatchHandler) {
            this->customMatchHandler(Result);
            return;
        }

        ASTContext* Context = Result.Context;
        auto& map = Result.Nodes.getMap();

        for (auto& m : map) {
            llvm::outs() << "Match found: " << m.first << "\n";
            m.second.dump(llvm::outs(), *Context);
            llvm::outs() << "Match end: " << m.first << "\n";
        }

        if (const auto* func = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplateDecl rewrite")) 
            matches[func->getID()].push_back(Result);

        if (const auto* func = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplateDecl2"))
            matches[func->getID()].push_back(Result);
    }

    void MatchHandler::onStartOfTranslationUnit() {
        matches.clear();
    }

    void MatchHandler::onEndOfTranslationUnit() {      
        for (auto& matchesPair : matches) {
            auto firstMatch = *matchesPair.second.begin();
            ASTContext* firstContext = firstMatch.Context;

            if (const auto* func = firstMatch.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplateDecl rewrite")) {
                const auto& funcDecl = func->getTemplatedDecl();

                FullSourceLoc FullLocation = firstContext->getFullLoc(funcDecl->getBeginLoc());
                FullLocation.getColumnNumber();
                SourceLocation s = funcDecl->getBeginLoc();

                auto& DiagnosticsEngine = firstContext->getDiagnostics();
                RewriterPointer Rewriter;
                if (DoRewrite) Rewriter = createRewriter(DiagnosticsEngine, *firstContext);

                string replaceText = "requires ";
                string noteText = "";
                int i = 0;
                for (auto & match : matchesPair.second) {
                    if (const auto* param = match.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("templateTypeParmDecl arithmetic")) {
                        const std::string& paramName = param->getName().str();
                        if (i++ != 0) replaceText += "&& ";
                        if (i != 0) noteText += " ";
                        replaceText += "std::is_arithmetic_v<" + paramName + "> ";
                        noteText += "(arithmetic)'" + paramName + "'";
                    }
                }
                
                // ToDo: \t....
                auto FixIt = FixItHint::CreateInsertion(funcDecl->getBeginLoc(), replaceText + "\n\t");
                auto& diag = firstContext->getDiagnostics();
                const auto diagID = diag.getCustomDiagID(clang::DiagnosticsEngine::Remark, "Consider adding concepts to template(s): %0");
                // So we destroy our builder to execute it..
                {
                    const auto& builder = diag.Report(func->getBeginLoc(), diagID);
                    builder.AddString(noteText);
                    builder.AddFixItHint(FixIt);
                }
    

                if (DoRewrite && Rewriter != nullptr) Rewriter->WriteFixedFiles();
            }

            if (auto* func = const_cast<clang::FunctionTemplateDecl*>( firstMatch.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplateDecl2") ) ) {
                const auto& funcDecl = func->getTemplatedDecl();
                //const auto& funcDecl = func->getTemplatedDecl();

                std::unique_ptr<ASTUnit> ToUnit = buildASTFromCode(
                    "#include <vector>\n template<class T1, class T2> bool foo(T1 x, T2 y); \n void ass(){ foo(6, 0); }", "to.cc");
                const clang::FunctionTemplateDecl* From = func;


               // Sema()
               
               /* auto asdff = func->specializations().begin();
                auto asdff2 = func->specializations().begin();
                asdff2++;

                asdff->dumpColor();
                asdff2->dumpColor();

                auto templateargs = asdff2->getTemplateSpecializationArgs();
                asdff->setFunctionTemplateSpecialization(func, templateargs, nullptr);
                
                asdff->dumpColor();

                auto hs = func->getTranslationUnitDecl()->decls_begin();
                while (1) {
                    hs++;
                    if (hs->getBeginLoc().isValid()) {
                        outs() << firstMatch.SourceManager->getCharacterData(hs->getBeginLoc());
                        hs->dumpColor();
                    }
                }*/
                //outs()<<firstMatch.SourceManager->getCharacterData();

                //ASTImporter Importer(ToUnit->getASTContext(), ToUnit->getFileManager(),
                //    From->getASTContext(), From->getASTContext().getSourceManager().getFileManager(),
                //   /*MinimalImport=*/false);
                ASTImporter Importer(
                    From->getASTContext(), From->getASTContext().getSourceManager().getFileManager(),
                    ToUnit->getASTContext(), ToUnit->getFileManager(), 
                    /*MinimalImport=*/false);

                auto MB = functionDecl(isExpansionInMainFile(), hasName("ass"), hasDescendant( declRefExpr().bind("proba"))).bind("bindStr");
                auto MatchRes = match(MB, ToUnit->getASTContext());
                FunctionDecl* Result = const_cast<FunctionDecl*>(MatchRes[0].template getNodeAs<FunctionDecl>("bindStr"));
                DeclRefExpr* Result2 = const_cast<DeclRefExpr*>(MatchRes[0].template getNodeAs<DeclRefExpr>("proba"));
                auto fundecl = Result;

                Result->dumpColor();
                Result2->dumpColor();

                auto ImportedOrErr = Importer.Import(Result2);
                if (!ImportedOrErr) {
                    llvm::Error Err = ImportedOrErr.takeError();
                    llvm::errs() << "ERROR: " << Err << "\n";
                    consumeError(std::move(Err));
                    return;
                }
                DeclRefExpr* Imported = (DeclRefExpr*)ImportedOrErr.get();

                
                Imported->dumpColor();
                Imported->getDecl()->dumpColor();

                Imported->getDecl()->getAsFunction()->dumpColor();

                outs()<<Imported->getDecl()->getAsFunction()->isInvalidDecl()<<" - \n";

                Imported->getDecl()->getAsFunction()->getPrimaryTemplate()->dumpColor();
                //ImportedOrErr.get()->dumpColor();

                //ImportedOrErr.get()->getdecl().dump();

                //Imported->getTranslationUnitDecl()->dump();

                /*if (llvm::Error Err = Importer.ImportDefinition(From)) {
                    llvm::errs() << "ERROR: " << Err << "\n";
                    consumeError(std::move(Err));
                    return 1;
                }
                llvm::errs() << "Imported definition.\n";*/
                //Imported->getTranslationUnitDecl()->dump();
            }
            
        }
    }

}