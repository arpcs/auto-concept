#include "ResourceTypes.h"


#include "llvm/Support/raw_ostream.h"

#include <string>
#include <string_view>
#include <fstream>

namespace auto_concept {
	using namespace std;
	Types::Types(string_view line){
		size_t startLoc = 0, tabLoc = 0;
		
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->name = line.substr(startLoc, tabLoc - startLoc);
			startLoc = tabLoc + 1;
		}
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->include = line.substr(startLoc, tabLoc - startLoc);
			startLoc = tabLoc + 1;
		}

		if (tabLoc != string::npos) {
			this->cpp20std = (line.substr(startLoc) == "TRUE");
			startLoc = tabLoc + 1;
		}
	}

	//CONCEPT NAME	DESCRIPTION	# ARGUMENTS	INCLUDES 	EXPOSITION ONLY	CPP20 STD	IS CONCEPT	PASSING TYPES
	Concepts::Concepts(string_view line){
		size_t startLoc = 0, tabLoc = 0, colonLoc = 0;

		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->name = line.substr(startLoc, tabLoc - startLoc);
			startLoc = tabLoc + 1;
		}
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->description = line.substr(startLoc, tabLoc - startLoc);
			startLoc = tabLoc + 1;
		}
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->numberOfArguments = atoi(string{ line.substr(startLoc, tabLoc - startLoc) }.c_str());
			startLoc = tabLoc + 1;
		}
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->include = line.substr(startLoc, tabLoc - startLoc);
			startLoc = tabLoc + 1;
		}
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->found = (line.substr(startLoc, tabLoc - startLoc) == "TRUE");
			startLoc = tabLoc + 1;
		}
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->cpp20std = (line.substr(startLoc, tabLoc - startLoc) == "TRUE");
			startLoc = tabLoc + 1;
		}
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->isConcept = (line.substr(startLoc, tabLoc - startLoc) == "TRUE");
			startLoc = tabLoc + 1;
		}

		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			string line2 = string{ line.substr(0, tabLoc) };
			colonLoc = 0;
			while (colonLoc != string::npos)
			{
				colonLoc = line2.find(';', startLoc);
				string type;

				if (colonLoc != string::npos) type = string{ line2.substr(startLoc, colonLoc - startLoc) };
				else						type = string{ line2.substr(startLoc) };
				if (type.find('\"') == 0) type = type.substr(1);
				if (type.find('\"') != string::npos) type = type.substr(0, type.length() - 1);
				if (type.length() > 0) this->specializations.push_back(type);
				startLoc = colonLoc + 1;
			}
			startLoc = tabLoc + 1;
		}

		while (tabLoc != string::npos)
		{
			tabLoc = line.find(';', startLoc);
			string type;
			
			if (tabLoc != string::npos) type = string{ line.substr(startLoc, tabLoc - startLoc) };
			else						type = string{ line.substr(startLoc) };
			if (type.find('\"') == 0) type = type.substr(1);
			if (type.find('\"') != string::npos) type = type.substr(0, type.length()-1);
			if (type.length() > 0) this->passingTypes.push_back(type);
			startLoc = tabLoc + 1;
		}
	}

	Resources::Resources(string_view typesFilename, string_view conceptsFilename) {
        vector<string> result;
		string fileName;
		for (int i = 0; i < 2; i++)
		{
			if (i == 0) fileName = "../AutoConcept/" + string{ typesFilename };
			else		fileName = "../AutoConcept/" + string{ conceptsFilename };
			ifstream file(fileName);
			if (!file.is_open()) {
				llvm::errs() << "ERROR: Can't open resources file!\n";
				return;
			}
			string line;
			getline(file, line);
			while (getline(file, line)) {
				string_view line_view = line;
				if (i == 0) this->types.	emplace_back(line_view);
				else		this->concepts.	emplace_back(line_view);
			}
		}
	}
}