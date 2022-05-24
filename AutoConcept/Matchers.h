#pragma once

#include <vector>
#include "clang/ASTMatchers/ASTMatchers.h"


namespace auto_concept {

    // Forward matchers
    std::vector<clang::ast_matchers::DeclarationMatcher> GetMatchersGuessed();
    inline std::vector<clang::ast_matchers::DeclarationMatcher> GetMatchers() { 
        return GetMatchersGuessed();
    }
}
