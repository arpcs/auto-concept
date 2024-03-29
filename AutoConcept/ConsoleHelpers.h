#pragma once


//#include <clang/ASTMatchers/ASTMatchers.h>
//#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <LLVM/Support/raw_ostream.h>

#include <string>
#include <vector>

namespace auto_concept {

	// An RAII object to track long proccesses and give feedback to user
	class ProgressBar {
		bool started = false;
		int lastProgress = 0;
		const int percentageSkip = 2;
	public:
		ProgressBar(std::string progressName, int numberOfSteps) :progressName{ progressName }, numberOfSteps{ numberOfSteps }  {
			Start();
		}
		~ProgressBar() {
			if (started) End();
		}

		void Start();
		void End();
		void Tick();
		void Step(int step);

		const std::string progressName;
		const int numberOfSteps;
		int currentSteps;
	};


	std::vector<std::string> GetWithProbeFiles(const std::vector<std::string>& files, const std::string suffix, std::vector<std::string>& injected);

}