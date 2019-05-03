#include "Finder.h"
#include "Configuration.h"
#include "FiniteAbelianGroups.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

#define NO_WARN_MBCS_MFC_DEPRECATION

using std::ifstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using json = nlohmann::json;


int main(int argc, char* argv[]) {

	// get json from a file and load up configurations
	try {

		string file_path = "C:\\Users\\benji\\Documents\\MathResearch2017\\SolutionFinder\\test10.json";
		ifstream inFile(file_path);
		if (!inFile) {
			throw "could not open file";
		}
		json root = json::parse(inFile);
		
		json::iterator iter;

		vector<Configuration> config_vec;

		for (iter = root.begin(); iter != root.end(); iter++) {
			Configuration temp = Configuration(*iter);
			config_vec.push_back(temp);
		}

		vector<AbelianGroupElement> all_groups = get_all_groups_of_order(16);

		vector<AbelianGroupElement>::iterator group_iter;

		// run algorithm
		for (group_iter = all_groups.begin(); group_iter != all_groups.end(); group_iter++) {
			FinderReturn result = find_solution(config_vec[0], *group_iter);
			cout << result << endl;
		}

		return 0;

	}
	catch (string err) {
		throw err;
	}

}