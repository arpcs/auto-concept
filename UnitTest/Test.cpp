#include "TestReader.h"

#include "gtest/gtest.h"
#include <iostream>

namespace auto_concept_test {

	using namespace std;

	TEST(FixItTests, FixItTests) {
		TestReader reader("FixItTests.cpp");

		int counter = 0;
		for (auto& testCase : reader)
		{
			std::cout << std::endl << "[Starting "<< ++counter << ". Concept Rewrite Test]" << std::endl << std::endl;
			EXPECT_EQ(runner(testCase.test, testCase.commandArgs), testCase.expected);
		}
	}

	TEST(CommandLineTests, CommandLineTests) {
		TestReader reader("CommandLineTests.cpp");

		int counter = 0;
		for (auto& testCase : reader)
		{
			std::cout << std::endl << "[Starting " << ++counter << ". Command Line Test]" << std::endl << std::endl;
			EXPECT_EQ(runner(testCase.test, testCase.commandArgs), testCase.expected);
		}
	}
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}