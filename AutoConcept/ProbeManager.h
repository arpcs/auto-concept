#pragma once

#include <unordered_map>
#include "MatchHandler.h"
#include "ResourceTypes.h"
#include "Guesser.h"
#include <memory>
#include <vector>

namespace auto_concept {

	// Function to inject template and include probes
	void InjectProbes(const std::unordered_map< int64_t, clang::SmallVector<clang::ast_matchers::MatchFinder::MatchResult> >& matches, std::shared_ptr<Resources> resources);

	// Function to interpret template and include probes
	void CollectProbes(const std::unordered_map< int64_t, clang::SmallVector<clang::ast_matchers::MatchFinder::MatchResult> >& matches, GuesserCollection::InnerType guessers);

}