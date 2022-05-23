#pragma once

#include <vector>
#include "clang/ASTMatchers/ASTMatchers.h"


namespace auto_concept {
    std::vector<clang::ast_matchers::DeclarationMatcher> GetMatchersGuessed();
    std::vector<clang::ast_matchers::DeclarationMatcher> GetMatchersDeduced();
    inline std::vector<clang::ast_matchers::DeclarationMatcher> GetMatchers() { 
        std::vector<clang::ast_matchers::DeclarationMatcher> ret;
        auto deduced = GetMatchersDeduced();
        auto guessed = GetMatchersGuessed();
        std::copy(guessed.begin(), guessed.end(), std::back_inserter(ret));
        return ret;
    }
}
