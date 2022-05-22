#pragma once
#include <string>
#include <functional>

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>


namespace auto_concept{
	using MatchFinder = clang::ast_matchers::MatchFinder;

	enum class AutoConceptTuState {
		InjectingProbes,
		CollectingResults,
		ActingOnResults
	};
	enum class AutoConceptGlobalState {
		InjectionPass,
		FinalPass
	};

	int RunApp(int argc, const char** argv);
	int RunAppOnVirtual(std::string& virtualFile, 
		std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher = {},
		std::function<void(const MatchFinder::MatchResult&)>	 customMatchHandler = {});

}