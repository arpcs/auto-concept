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

		std::vector<std::optional<SpecializedConcept>> GetFittingConcepts() {
			using namespace std;

			if (!Santalyzer()) return {};

			const int tParams = templateSpecs.begin()->types.size();
			std::vector<std::optional<SpecializedConcept>> ret;
			ret.resize(tParams);

			unordered_map<std::string, size_t> typesInResources;
			for (size_t i = 0; i < resources->types.size(); i++)
			{
				typesInResources[resources->types[i].name] = i;
			}

			
			size_t tParamIndex = 0;
			for (const auto& specTypeMap : onTemplateLocationTypes) {	
				int minNumberOfPassingTypes = INT_MAX;
				int conceptWhere = -1;

				for (size_t i = 0; i < resources->concepts.size(); i++) {

					// Check if concepts says false when the specialization fails
					// And if the concepts says true when the specialization passes
					bool goodFit = true;
					for (const auto& specType : specTypeMap) {
						if (!typesInResources.contains(specType.second.typeName)) continue;
						if (specType.second.validity == Validity::bad) {
							if (resources->concepts[i].passingTypes.contains(specType.second.typeName)) {
								goodFit = false;
								break;
							}
						}
						if (specType.second.validity == Validity::good) {
							if (!resources->concepts[i].passingTypes.contains(specType.second.typeName)) {
								goodFit = false;
								break;
							}
						}
					}

					if (goodFit) {
						if (minNumberOfPassingTypes > resources->concepts[i].passingTypes.size()) {
							minNumberOfPassingTypes = resources->concepts[i].passingTypes.size();
							conceptWhere = i;
						}
					}
				}

				if (conceptWhere != -1 && minNumberOfPassingTypes > 0) {
					ret[tParamIndex] = SpecializedConcept{ resources->concepts[conceptWhere] , templateParams[tParamIndex] };
				}

				tParamIndex++;
			}

			return ret;
		}

		Guesser(std::shared_ptr<Resources> resources) : resources{ resources } {};

	private:

		// Were the types good or bad?
		// Returning false if it's not black or white
		bool Santalyzer() {

			if (templateSpecs.size() == 0 || templateSpecs.begin()->types.size() == 0) return false;

			const int tParams = templateSpecs.begin()->types.size();
			onTemplateLocationTypes.resize(tParams);

			for (size_t tParam = 0; tParam < tParams; tParam++)
			{
				for (const auto& templateSpec : templateSpecs)
				{
					if (templateSpec.types.size() != tParams) return false;
					auto loc = onTemplateLocationTypes[tParam].find(templateSpec.types[tParam].typeName);
					if (loc == onTemplateLocationTypes[tParam].end()) {
						DiscriminatedType  dType = DiscriminatedType{ templateSpec.types[tParam].typeName };
						if (templateSpec.good) dType.validity = Validity::good;
						onTemplateLocationTypes[tParam][templateSpec.types[tParam].typeName] = dType;
					}
					else {
						if (templateSpec.good) loc->second.validity = Validity::good;
					}
				}
			}


			bool changed = true;
			while (changed) {
				changed = false;

				for (const auto& templateSpec : templateSpecs)
				{
					if (templateSpec.good) {
						for (size_t outer = 0; outer < tParams; outer++)
						{
							auto locOuter = onTemplateLocationTypes[outer].find(templateSpec.types[outer].typeName);
							// If types interact with each other
							if (locOuter->second.validity == Validity::bad) return false;
						}
						continue;
					}
					for (size_t outer = 0; outer < tParams; outer++)
					{
						bool allGoodExceptMe = true;
						auto locOuter = onTemplateLocationTypes[outer].find(templateSpec.types[outer].typeName);
						for (size_t inner = 0; inner < tParams; inner++)
						{
							if (inner == outer) continue;
							auto locInner = onTemplateLocationTypes[inner].find(templateSpec.types[inner].typeName);

							if (locInner->second.validity != Validity::good) {
								allGoodExceptMe = false;
								break;
							}
						}

						if (allGoodExceptMe) {

							// If types interact with each other
							if (locOuter->second.validity == Validity::good) return false;
							if (locOuter->second.validity != Validity::bad) {
								locOuter->second.validity = Validity::bad;
								changed = true;
							}
						}
					}
				}

			}

			return true;
		}

	};



}
