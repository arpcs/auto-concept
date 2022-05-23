#pragma once

#include "ResourceTypes.h"

namespace auto_concept {

	// Mathces the content of Concepts.txt file with the content of Types.txt
	// And generates fields into Concepts.txt
	void FillMissingResources(Resources& resources);

}