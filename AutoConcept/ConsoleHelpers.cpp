
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
		if (!started) return;
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
	}


	std::vector<std::string> GetWithProbeFiles(const std::vector<std::string>& files, const std::string suffix, std::vector<std::string>& injected) {
		std::vector<std::string> changedArgs = files;
		for (const auto& filename : files)
		{
			const std::string newFilename = filename.substr(0, filename.find_last_of('.') + 1) + suffix + filename.substr(filename.find_last_of('.'));
			for (auto it = changedArgs.begin(); it != changedArgs.end(); it++) {
				if (std::string(*it) == filename) {
					changedArgs.insert(it + 1, newFilename);
					injected.push_back(newFilename);
					break;
				}
			}

		}

		return changedArgs;
	}
}