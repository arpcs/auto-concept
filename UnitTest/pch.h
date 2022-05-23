//
// pch.h
//

#pragma once

#include <string>
#include <fstream>
#include <iterator>
#include <cstddef>
#include <array>
#include <vector>

namespace auto_concept_test {

	std::string runner(std::string input, const std::vector<std::string>& args);

	class TestReader {
	public:
		struct Test {
			const std::string test;
			const std::string expected;
			const std::vector<std::string> commandArgs;
		};
	private:
		const std::array<std::string, 3> separators = {
			"// [Setup]",
			"// [Test]",
			"// [Expected]" };

		enum class State
		{
			setup,
			test,
			expected
		};

		std::ifstream file;
		std::vector<Test> tests;
	public:

		TestReader(const std::string& a) : file{ a } {
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
					tests.push_back({ setup + test, setup + expected , commandArgs});
				}
			};

			while (std::getline(file, line)) {
				bool skip = false;
				if (line.starts_with("// [Comment]")) continue;
				if (line.starts_with("// [Arg] ")) {
					commandArgs.push_back(line.substr(std::string("// [Arg] ").length()));
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

		const auto begin() {
			return tests.begin();
		}
		const auto end() {
			return tests.end();
		}
	};
}