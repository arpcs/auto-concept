#pragma once

#include "clang/ASTMatchers/ASTMatchers.h"
#include <vector>


// Matchers have many dependencies, so they build pretty slow
// To mitigate this we use this file to build matchers dynamically from string or we just pass the matchers
// This is controlled by the AUTOCONCEPT_FAST_DYNAMIC_BUILD macro.
#if defined(AUTOCONCEPT_FAST_DYNAMIC_BUILD)

inline std::vector<clang::ast_matchers::DeclarationMatcher> GetMatchers() {
    return {};
}

#else

std::vector<clang::ast_matchers::DeclarationMatcher> GetMatchers();

#endif