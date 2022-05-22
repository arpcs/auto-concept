#pragma once

#include <unordered_map>
#include "MatchHandler.h"
#include "ResourceTypes.h"
#include <memory>

namespace auto_concept {
	void InjectProbes(const std::unordered_map< int64_t, clang::SmallVector<clang::ast_matchers::MatchFinder::MatchResult> >& matches, std::shared_ptr<Resources> resources);
}