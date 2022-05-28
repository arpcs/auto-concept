#include "ResourceTypes.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include "llvm/ADT/SmallVector.h"

#include "Guesser.h"
#include "CommandLineOpts.h"

namespace auto_concept {

	using namespace std;

	vector<optional<SpecializedConcept>> Guesser::GetFittingConcepts(shared_ptr<Resources> resources) {
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

				if (resources->concepts[i].passingTypes.size() <= 1) continue;
				
				// Check for concept-type fitting
				int allowedRemaining = CLOptions::MaxAllowOption;
				int preventedRemaining = CLOptions::MaxPreventOption;
				for (const auto& specType : specTypeMap) {
					if (!typesInResources.contains(specType.second.typeName)) continue;

					// Check if concepts says false when the specialization fails
					if (specType.second.validity == Validity::bad) {
						if (resources->concepts[i].passingTypes.contains(specType.second.typeName)) {
							if (!CLOptions::contains(CLOptions::IgnoreTypeOption, specType.second.typeName)) {
								allowedRemaining--;
								if (CLOptions::contains(CLOptions::TestConceptOption, resources->concepts[i].name)) {
									if (allowedRemaining > -5)
										llvm::outs() << "[Test concept] * Concept '" + resources->concepts[i].name + "' allowed semantically incorrect specialization for template param index: " + to_string(tParamIndex) + "\tand type: " + specType.second.typeName + "\n";
								}
								else if (allowedRemaining < 0) break;
							}
						}
					}

					// Check if the concepts says true when the specialization passes
					if (specType.second.validity == Validity::good) {
						if (!resources->concepts[i].passingTypes.contains(specType.second.typeName)) {
							if (!CLOptions::contains(CLOptions::IgnoreTypeOption, specType.second.typeName)) {
								preventedRemaining--;
								if (CLOptions::contains(CLOptions::TestConceptOption, resources->concepts[i].name)) {
									if (preventedRemaining > -5)
										llvm::outs() << "[Test concept] * Concept '" + resources->concepts[i].name + "' prevented semantically correct specialization for template param index: " + to_string(tParamIndex) + "\tand type: " + specType.second.typeName + "\n";
								}
								else if (preventedRemaining < 0) break;
							}
						}
					}
				}
				if (allowedRemaining <= -5) llvm::outs() << "[Test concept] * Concept '" + resources->concepts[i].name + "' allowed " + to_string(-allowedRemaining - 5) + " more semantically incorrect specialization for template param index: " + to_string(tParamIndex) + "\n";
				if (preventedRemaining <= -5) llvm::outs() << "[Test concept] * Concept '" + resources->concepts[i].name + "' prevented " + to_string(-preventedRemaining - 5) + " more semantically correct specialization for template param index: " + to_string(tParamIndex) + "\n";

				if (allowedRemaining>=0 && preventedRemaining>=0) {

					// Print stats for concept
					if (CLOptions::contains(CLOptions::TestConceptOption, resources->concepts[i].name)) llvm::outs() << 
						"[Test concept] * Concept '" + resources->concepts[i].name + "' is a good fit for template param index: " + to_string(tParamIndex)
						+"\tand have this many allowed types: "+to_string(resources->concepts[i].passingTypes.size())+"\tprevented: "<< 
						resources->types.size()- resources->concepts[i].passingTypes.size() <<"\n";

					if (minNumberOfPassingTypes > resources->concepts[i].passingTypes.size()) {
						minNumberOfPassingTypes = resources->concepts[i].passingTypes.size();
						conceptWhere = i;
					}
				}
			}

			if (conceptWhere != -1 && minNumberOfPassingTypes > 0 && minNumberOfPassingTypes <= resources->types.size() - CLOptions::MinPreventOption) {
				ret[tParamIndex] = SpecializedConcept{ resources->concepts[conceptWhere] , templateParams[tParamIndex] };
			}

			tParamIndex++;
		}

		return ret;
	}

	// Were the types good or bad?
	// Returning false if it's not black or white
	bool Guesser::Santalyzer() {

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

						// If types interact with each other return now
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
}
