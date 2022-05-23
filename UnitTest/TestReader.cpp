//
// pch.cpp
//

#include "TestReader.h"
#include "../AutoConcept/AutoConcept.h"
#include "gtest/gtest.h"

namespace auto_concept_test {
	using namespace auto_concept;

	std::string runner(std::string input, const std::vector<std::string> & args = {}) {
		RunAppOnVirtual(input, args);
		return input;
	}

	TestReader::TestReader(const std::string& a) : file{ a } {
		if (!file.is_open()) return;

		std::string setup, test, expected, line;
		std::vector<std::string> commandArgs;
		State state;

		auto startBuffer = [&](State newState) {
			switch (newState)
			{
			case State::setup:
				setup.clear();
			case State::test:
				test.clear();
				commandArgs.clear();
			case State::expected:
				expected.clear();
			}
			state = newState;
		};

		auto pushToBuffers = [&]() {
			switch (state)
			{
			case State::setup:
				setup += line + '\n';
				break;
			case State::test:
				test += line + '\n';
				break;
			case State::expected:
				expected += line + '\n';
				break;
			}
		};

		auto finishBuffers = [&]() {
			if (state == State::expected) {
				tests.push_back({ setup + test, setup + expected , commandArgs });
			}
		};

		while (std::getline(file, line)) {
			bool skip = false;
			if (line.starts_with(commentSeparator)) continue;
			if (line.starts_with(argumentSeparator)) {
				commandArgs.push_back(line.substr(argumentSeparator.length()));
				continue;
			}
			for (int i = 0; i < separators.size(); i++)
			{
				if (line.starts_with(separators[i])) {
					finishBuffers();
					startBuffer(static_cast<State>(i));
					skip = true;
					break;
				}
			}
			if (!skip) pushToBuffers();
		}
		if (setup.size() > 0 || test.size() > 0) finishBuffers();

	}
}
