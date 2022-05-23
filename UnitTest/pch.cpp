//
// pch.cpp
//

#include "pch.h"
#include "../AutoConcept/AutoConcept.h"

#include "gtest/gtest.h"

namespace auto_concept_test {
	using namespace auto_concept;

	std::string runner(std::string input, const std::vector<std::string> & args = {}) {
		RunAppOnVirtual(input, args);
		return input;
	}
}
