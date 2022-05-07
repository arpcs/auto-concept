
// Matchers have many dependencies, so they build pretty slow
// To mitigate this we use this file to build matchers dynamically from string or we just pass the matchers
// This is controlled by the AUTOCONCEPT_FAST_DYNAMIC_BUILD macro.
// We turn on intellisense on this file nevertheless to still get it's errors
#if defined(__INTELLISENSE__) || !defined(AUTOCONCEPT_FAST_DYNAMIC_BUILD)

#include "Matchers.h"

#include "clang/ASTMatchers/Dynamic/Parser.h"
#include "clang/ASTMatchers/Dynamic/Parser.h"
#include "clang/Basic/CharInfo.h"
#include "clang/Tooling/NodeIntrospection.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include <string>
#include <set>
#include <vector>

using namespace llvm;
using namespace std;
using namespace clang::ast_matchers;
using namespace clang::ast_matchers::dynamic;

vector<DeclarationMatcher> GetMatchers() {

    // Don't change the name "matchers"!
    vector<DeclarationMatcher> matchers;

    matchers.push_back(
        functionTemplateDecl(
        isExpansionInMainFile(),
        has(
            functionDecl()
        )
        ).bind("function-to-autoconcept")
    );
    return matchers;
}

#endif