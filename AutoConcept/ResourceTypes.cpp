#include "ResourceTypes.h"


#include "llvm/Support/raw_ostream.h"

#include <string>
#include <string_view>
#include <fstream>
#include <unordered_set>
#include <filesystem>

namespace auto_concept {
	using namespace std;

	constexpr bool TESTING = false;

	Type::Type(string_view line){
		size_t startLoc = 0, tabLoc = 0;
		
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->name = line.substr(startLoc, tabLoc - startLoc);
			startLoc = tabLoc + 1;
		}
		tabLoc = line.find('\t', startLoc);
		if (tabLoc != string::npos) {
			this->desugaredName = line.substr(startLoc, tabLoc - startLoc);
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
	std::string Type::toStr() {
		std::string res;
		res += this->name + "\t"s;
		res += this->desugaredName + "\t"s;
		res += this->include + "\t"s;
		res += (this->cpp20std ? "TRUE"s : "FALSE"s);
		return res;
	}

	Concept::Concept(string_view line){
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
				if (type.length() > 0) this->specializations.insert(type);
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
			if (type.length() > 0) this->passingTypes.insert(type);
			startLoc = tabLoc + 1;
		}
	}

	std::string Concept::toStr() {
		std::string res;
		res += this->name + "\t"s;
		res += this->description + "\t"s;
		res += std::to_string(this->numberOfArguments) + "\t"s;
		res += this->include + "\t"s;
		res += (this->found ? "TRUE":"FALSE") + "\t"s;
		res += (this->cpp20std ? "TRUE" : "FALSE") + "\t"s;
		res += (this->isConcept ? "TRUE" : "FALSE") + "\t"s;
		int i=0;
		for (auto& spec: this->specializations)
		{
			if (i != 0) res += ";";
			res += spec;
			i++;
		}
		res += "\t";
		i = 0;
		for (auto& spec : this->passingTypes)
		{
			if (i != 0) res += ";";
			res += spec;
			i++;
		}
		return res;
	}


	Resources::Resources(string_view typesFilename, string_view conceptsFilename) {
		string fileName;
		for (int i = 0; i < 2; i++)
		{
			if (std::filesystem::exists(string{ typesFilename })) {
				if (i == 0) fileName = string{ typesFilename };
				else		fileName = string{ conceptsFilename };
			}
			if (std::filesystem::exists("../AutoConcept/" + string{ typesFilename })) {
				if (i == 0) fileName = "../AutoConcept/" + string{ typesFilename };
				else		fileName = "../AutoConcept/" + string{ conceptsFilename };
			}
			if (std::filesystem::exists("../../AutoConcept/" + string{ typesFilename })) {
				if (i == 0) fileName = "../../AutoConcept/" + string{ typesFilename };
				else		fileName = "../../AutoConcept/" + string{ conceptsFilename };
			}
			ifstream file(fileName);
			if (!file.is_open()) {
				llvm::errs() << "ERROR: Can't open resources file!\n";
				return;
			}
			string line;
			getline(file, line);
			if (i == 0) this->typesFirstRow = line;
				else this->conceptsFirstRow = line;
			while (getline(file, line)) {
				string_view line_view = line;
				if (i == 0) this->types.	emplace_back(line_view);
				else		this->concepts.	emplace_back(line_view);
				if constexpr (TESTING) {
					if (i == 0) assert(this->types.		back().toStr() == line);
					else		assert(this->concepts.	back().toStr() == line);
				}
			}
		}
	}

	void Resources::Save(std::string_view typesFilename, std::string_view conceptsFilename) {
		string fileName;
		for (int i = 0; i < 2; i++)
		{
			if (std::filesystem::exists(string{ typesFilename })) {
				if (i == 0) fileName = string{ typesFilename };
				else		fileName = string{ conceptsFilename };
			}
			if (std::filesystem::exists("../AutoConcept/" + string{ typesFilename })) {
				if (i == 0) fileName = "../AutoConcept/" + string{ typesFilename };
				else		fileName = "../AutoConcept/" + string{ conceptsFilename };
			}
			if (std::filesystem::exists("../../AutoConcept/" + string{ typesFilename })) {
				if (i == 0) fileName = "../../AutoConcept/" + string{ typesFilename };
				else		fileName = "../../AutoConcept/" + string{ conceptsFilename };
			}

			ofstream file(fileName);
			if (!file.is_open()) {
				llvm::errs() << "ERROR: Can't write resources file!\n";
				return;
			}
			if (i == 0) {
				file.write(this->typesFirstRow.c_str(), this->typesFirstRow.size());
				file.write("\n", 1);
				bool first = true;
				for (auto& row : this->types) {
					if (!first) file.write("\n", 1);
					first = false;
					string rowStr = row.toStr();
					file.write(rowStr.c_str(), rowStr.size());
				}
			}
			else {
				file.write(this->conceptsFirstRow.c_str(), this->conceptsFirstRow.size());
				file.write("\n", 1);
				bool first = true;
				for (auto& row : this->concepts) {
					if (!first) file.write("\n", 1);
					first = false;
					string rowStr = row.toStr();
					file.write(rowStr.c_str(), rowStr.size());
				}
			}
		}
	}
}