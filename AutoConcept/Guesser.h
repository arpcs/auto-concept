#pragma once
#include "ResourceTypes.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>
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
	public:

		struct SpecType
		{
			std::string typeName;
			SpecType(std::string typeName) : typeName{ typeName } {}
			auto operator<=>(const SpecType&) const = default;
		};
		struct SpecTypes {
			std::vector<SpecType> types;
			bool good = true;
			auto operator<=>(const SpecTypes&) const = default;
		};


		llvm::SmallVector<std::string> templateParams;
		std::set<SpecTypes> templateSpecs;

		std::vector<std::optional<SpecializedConcept>> GetFittingConcepts(std::shared_ptr<Resources> resources);

		Guesser() {};

	private:

		// Were the types good or bad?
		// Returning false if it's not black or white
		bool Santalyzer();

	};

	struct GuesserCollection {
		using InnerType = std::shared_ptr<std::unordered_map<std::string, Guesser>>;
		std::vector<InnerType> guessers;
		InnerType get(int index) {
			assert(index >= 0 && index <= guessers.size());
			if (guessers.size() <= index) {
				guessers.resize(index + 1);
				guessers[index] = std::make_shared <std::unordered_map<std::string, Guesser >>();
			}
			return guessers[index];
		}
	};



}
