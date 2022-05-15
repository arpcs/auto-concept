#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace auto_concept {
	class Types {
	public:
		Types(std::string_view line);
		std::string name;
		std::string include;
		bool cpp20std = false;
	};

	class Concepts {
	public:
		Concepts(std::string_view line);
		std::string name;
		std::string description;
		int numberOfArguments = 0;
		std::string include;
		bool expositionOnly = false;
		bool cpp20std = false;
		bool isConcept = false;
		std::vector<std::string> passingTypes;
	};

	struct ResourceState {
		bool haveTypes = false;
	};

	// Main resource handler
	class Resources {
	public:
		Resources(std::string_view typesFilename = "Types.txt", std::string_view conceptsFilename = "Concepts.txt");
		std::vector<Types> types;
		std::vector<Concepts> concepts;
	};


}
