#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace auto_concept {

	/*struct ResourceState {
		bool typesGood = false;
		bool conceptsGood = false;
	};*/

	class Types {
	public:
		Types(std::string_view line);
		std::string toStr();
		std::string name;
		std::string include;
		bool cpp20std = false;
	};

	class Concepts {
	public:
		Concepts(std::string_view line);
		std::string toStr();

		std::string name;
		std::string description;
		int numberOfArguments = 0;
		std::string include;
		bool found = false;
		bool cpp20std = false;
		bool isConcept = false;
		std::vector<std::string> specializations;
		std::vector<std::string> passingTypes;
	};

	// Main resource handler
	class Resources {
	public:
		//ResourceState resourceState;
		Resources(std::string_view typesFilename = "Types.txt", std::string_view conceptsFilename = "Concepts.txt");
		void Save(std::string_view typesFilename = "Types.txt", std::string_view conceptsFilename = "Concepts.txt");

		std::string typesFirstRow, conceptsFirstRow;

		std::vector<Types> types;
		std::vector<Concepts> concepts;
	};


}

