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

// Standard includes
#include <cassert>
#include <memory>
#include <string>
#include <type_traits>

#include <iostream>
#include <concepts>

#include "MatchHandler.h"
#include "Consumer.h"
#include "Matchers.h"


namespace auto_concept {
    using namespace clang::ast_matchers;
    using namespace clang::tooling;
    using namespace clang;
    using namespace llvm;

    void Consumer::Consume() noexcept {
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
            matchFinder.addMatcher(matcher, &handler);

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


            auto matchers = GetMatchers();
            for (const auto& matcher : matchers)
            {
                matchFinder.addMatcher(matcher, &handler);
            }
            //matchFinder.addMatcher(matcher, &handler);

            //matchFinder.addMatcher(matcher, &handler);
            //Finder.matchAST(Context);
        }

}