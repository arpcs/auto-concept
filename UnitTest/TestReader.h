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

		const std::string commentSeparator = "// [Comment]";
		const std::string argumentSeparator = "// [Arg] ";

		enum class State
		{
			setup,
			test,
			expected
		};

		std::ifstream file;
		std::vector<Test> tests;
	public:

		TestReader(const std::string& a);

		const auto begin() {
			return tests.begin();
		}
		const auto end() {
			return tests.end();
		}
	};
}