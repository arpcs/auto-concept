#pragma once


//#include <clang/ASTMatchers/ASTMatchers.h>
//#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <LLVM/Support/raw_ostream.h>

#include <string>

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

	void Start() {
		started = true;
		currentSteps = 0;
		lastProgress = -percentageSkip;
		llvm::outs() << "Starting job: " << progressName << "\n";
	}
	void End() {
		for (int i = currentSteps; i < numberOfSteps-1; i++) Tick();
		started = false;
		currentSteps = numberOfSteps;
		llvm::outs() <<"\n" << "Finished job: " << progressName << "\n\n";
	}
	void Tick() {
		Step(1);
	}
	void Step(int step) {
		if (!started) return;
		currentSteps+= step;
		while (currentSteps * 100 / numberOfSteps >= lastProgress + percentageSkip) {
			if (lastProgress / 10 != (lastProgress + percentageSkip) / 10 || lastProgress < 0) {
				llvm::outs() << ((currentSteps * 100 / numberOfSteps) / 10);
			}
			else {
				llvm::outs() << '.';
			}
			lastProgress += percentageSkip;
		}
		if (numberOfSteps <= currentSteps) End();
	}

	const std::string progressName;
	const int numberOfSteps;
	int currentSteps;
};

