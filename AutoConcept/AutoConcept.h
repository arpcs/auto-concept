#pragma once
#include <string>
#include <functional>

#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>


namespace auto_concept{
	using MatchFinder = clang::ast_matchers::MatchFinder;

	// Translation unit states
	enum class AutoConceptTuState {
		InjectingProbes,
		CollectingResults,
		ActingOnResults
	};

	// App states
	enum class AutoConceptGlobalState {
		InjectionPass,
		FinalPass
	};

	// App constants
	struct Constants {
		static inline const std::string virtualFileIn = "VirtualInFile.cpp";
		static inline const std::string virtualSuffix = "VirtualOut";
		static inline const std::string virtualFileOut = virtualFileIn.substr(0, virtualFileIn.find_last_of('.')) + "." + virtualSuffix + ".cpp";
		static inline const std::string injectionSuffix = "AutoConceptTempFile";
	};

	// Running app on console arguments
	int RunApp(int argc, const char** argv);

	// Running app on console arguments
	int RunApp(std::string& virtualFile,const std::vector<std::string> args = {},
		std::function<clang::ast_matchers::DeclarationMatcher()> customMatcher = {},
		std::function<void(const MatchFinder::MatchResult&)>	 customMatchHandler = {});

	// Function for deleting temp files and injected probes
	void DeleteTempFiles(bool afterTollRun, int pass, const std::vector<std::string>& injectedFiles);

}