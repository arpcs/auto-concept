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
#include "llvm/Support/WithColor.h"

// Standard includes
#include <cassert>
#include <memory>
#include <string>
#include <type_traits>

#include <iostream>
#include <concepts>
#include <variant>

#include "MatchHandler.h"
#include "Consumer.h"
#include "Matchers.h"


namespace auto_concept {
    using namespace clang::ast_matchers::internal;
    using namespace clang::ast_matchers::dynamic;
    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;
    using namespace std;

    // AST Matcher consumer
    void Consumer::Consume() noexcept {
            Diagnostics Diag;        
            if (this->handler.customMatcher) {
                matchFinder.addMatcher(this->handler.customMatcher(), &handler);
                return;
            }
            auto matchers = GetMatchers();
            for (const auto& matcherRaw : matchers)
            {
                variant<string, DeclarationMatcher> matcher = matcherRaw;
                if constexpr        (same_as<std::remove_cvref_t<decltype(matcherRaw)>, DeclarationMatcher>) {
                    matchFinder.addMatcher(get<DeclarationMatcher>(matcher), &handler);
                }
                else if constexpr   (same_as< std::remove_cvref_t<decltype(matcherRaw)>, string>) {
                    string matcherStr = get<string>(matcher);
                    StringRef matcherStrRef = matcherStr;
                    auto parsedMatcher = Parser::parseMatcherExpression(matcherStrRef, &Diag);
                    if (parsedMatcher.hasValue()) {
                        matchFinder.addDynamicMatcher(parsedMatcher.getValue(), &handler);
                    }
                    else {
                        llvm::WithColor color(llvm::errs(), raw_ostream::Colors::RED);
                        llvm::errs() << "ERROR: Unable to parse the following matcher:\n";
                        llvm::errs() << "\""<<matcherStrRef << "\"\n";
                    }
                }
            }

        }

    void Consumer::HandleTranslationUnit(clang::ASTContext& Context) {

        matchFinder.matchAST(Context);

        // Generate a temp out file for the probe manager even if we have no matches
        if (!handler.haveMoreThanZeroMatch && handler.tuState == AutoConceptTuState::InjectingProbes && handler.DoRewrite) {
            MatchHandler::RewriterPointer Rewriter = nullptr;
            auto& DiagnosticsEngine = Context.getDiagnostics();
            Rewriter = handler.createRewriter(DiagnosticsEngine, Context);

            auto fileStartPos = DiagnosticsEngine.getSourceManager().getLocForStartOfFile(DiagnosticsEngine.getSourceManager().getMainFileID());
            auto FixIt = FixItHint::CreateInsertion(fileStartPos, "// [AutoConcept] No matches found\n");
            const auto diagID = DiagnosticsEngine.getCustomDiagID(clang::DiagnosticsEngine::Remark, "No template matches found");
            // So we destroy our builder to execute it..
            {
                const auto& builder = DiagnosticsEngine.Report(diagID);
                builder.AddFixItHint(FixIt);
            }

            Rewriter->WriteFixedFiles();
        }
    }
}