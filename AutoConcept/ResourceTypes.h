#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include "llvm/ADT/SmallVector.h"

namespace auto_concept {

	/*struct ResourceState {
		bool typesGood = false;
		bool conceptsGood = false;
	};*/

	class Type {
	public:
		Type(std::string_view line);
		std::string toStr();
		std::string name;
		std::string include;
		bool cpp20std = false;
	};

	class Concept {
	public:
		Concept(std::string_view line);
		std::string toStr();

		std::string name;
		std::string description;
		int numberOfArguments = 0;
		std::string include;
		bool found = false;
		bool cpp20std = false;
		bool isConcept = false;
		std::unordered_set<std::string> specializations;
		std::unordered_set<std::string> passingTypes;
	};

	struct SpecializedConcept {
		Concept conc;
		std::string templateParamNames;
		SpecializedConcept(const Concept& conc, const std::string& templateParamNames) : 
			conc{ conc }, templateParamNames{ templateParamNames } {}
	};


	// Main resource handler
	class Resources {
	public:
		//ResourceState resourceState;
		Resources(std::string_view typesFilename = "Types.txt", std::string_view conceptsFilename = "Concepts.txt");
		void Save(std::string_view typesFilename = "Types.txt", std::string_view conceptsFilename = "Concepts.txt");

		std::string typesFirstRow, conceptsFirstRow;

		std::vector<Type> types;
		std::vector<Concept> concepts;
	};


}

