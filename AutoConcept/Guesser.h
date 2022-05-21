#pragma once
#include "ResourceTypes.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include "llvm/ADT/SmallVector.h"

namespace auto_concept {

	class Guesser {
		enum class Validity
		{
			unknown,
			good,
			bad
		};
		struct DiscriminatedType {
			std::string typeName;
			Validity validity;
			DiscriminatedType(std::string typeName, Validity validity) : typeName{ typeName }, validity{ validity } {}
			DiscriminatedType(std::string typeName) : typeName{ typeName }, validity{ Validity::unknown } {}
			DiscriminatedType() : typeName{ "AUTO_CONCEPT_WRONG_TYPE" }, validity{ Validity::unknown } {}
		};
		std::vector<std::unordered_map<std::string, DiscriminatedType>> onTemplateLocationTypes;
		std::shared_ptr<Resources> resources;
	public:

		struct SpecType
		{
			std::string typeName;
			SpecType(std::string typeName) : typeName{ typeName } {}
		};
		struct SpecTypes {
			llvm::SmallVector<SpecType> types;
			bool good = true;
		};


		llvm::SmallVector<std::string> templateParams;
		std::vector<SpecTypes> templateSpecs;

		std::vector<std::optional<SpecializedConcept>> GetFittingConcepts();

		Guesser(std::shared_ptr<Resources> resources) : resources{ resources } {};

	private:

		// Were the types good or bad?
		// Returning false if it's not black or white
		bool Santalyzer();

	};



}
