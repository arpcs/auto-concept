#include "pch.h"

#include "gtest/gtest.h"

namespace auto_concept_test {

	using namespace std;

	TEST(TestCaseName, TestName) {


		//EXPECT_EQ(runner("template<typename T> void f(T t) {return t;}"), "template<typename T> void f(T t) {return t;}");

		TestReader reader("RewriteTestsDeduced.cpp");

		for (auto& testCase : reader)
		{
			EXPECT_EQ(runner(testCase.test, testCase.commandArgs), testCase.expected);
		}

		EXPECT_TRUE(true);
	}
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}