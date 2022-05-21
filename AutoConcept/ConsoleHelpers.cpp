
#include <LLVM/Support/raw_ostream.h>
#include <string>

#include "ConsoleHelpers.h"


namespace auto_concept {

	void ProgressBar::Start() {
		started = true;
		currentSteps = 0;
		lastProgress = -percentageSkip;
		llvm::outs() << "Starting job: " << progressName << "\n";
	}
	void ProgressBar::End() {
		for (int i = currentSteps; i < numberOfSteps - 1; i++) Tick();
		started = false;
		currentSteps = numberOfSteps;
		llvm::outs() << "\n" << "Finished job: " << progressName << "\n\n";
	}
	void ProgressBar::Tick() {
		Step(1);
	}
	void ProgressBar::Step(int step) {
		if (!started) return;
		currentSteps += step;
		while ((currentSteps * 100 / numberOfSteps >= lastProgress + percentageSkip) && lastProgress + percentageSkip < 100) {
			if (lastProgress / 10 != (lastProgress + percentageSkip) / 10 || lastProgress < 0) {
				llvm::outs() << (lastProgress + percentageSkip) / 10;
			}
			else {
				llvm::outs() << '.';
			}
			lastProgress += percentageSkip;
		}
		if (numberOfSteps <= currentSteps) End();
	}

}