#pragma once

#include <vector>
#include <map>
#include "FiniteAbelianGroups.h"
#include "Configuration.h"
#include <algorithm>
#include <iostream>

using std::map;
using std::ostream;
using std::cout;
using std::endl;

template <typename V>
void pop_front(V & v)
{
	if (!v.empty()) {
		v.erase(v.begin());
	}
}

struct FinderReturn {
	bool IsMagic;
	map<Point*, AbelianGroupElement> solution;
};

void assign_point(vector<Point*>& point_set,
	vector<Line*>& line_set,
	vector<AbelianGroupElement> element_set,
	map<Point*, AbelianGroupElement> map,
	FinderReturn& result);

string dump_map(map<Point*, AbelianGroupElement>& mapex);

ostream& operator<<(ostream& os, FinderReturn& to_print) {
	if (to_print.IsMagic == false) {
		os << "Failure" << endl;
	}
	else {
		os << dump_map(to_print.solution) << endl;
	}
	return os;
}

FinderReturn find_solution(Configuration& config, AbelianGroupElement representative) {

	vector<AbelianGroupElement> group_elements = representative.get_full_group();

	vector<Point*> point_set = config.get_points();
	vector<Line*> line_set = config.get_lines();

	map<Point*, AbelianGroupElement> map;

	FinderReturn result;

	assign_point(point_set, line_set, group_elements, map, result);

	return result;

}

bool filled_lines_are_invariant(vector<Line*> line_set, map<Point*, AbelianGroupElement> map) {
	// take in a set of lines and a partially full set of points
	
	// find which lines have all points assigned

	vector<Line*> filled_lines;

	vector<Line*>::iterator iter;

	for (iter = line_set.begin(); iter < line_set.end(); iter++) {
		if ( (*(*iter)).is_full()) { // double dereference for line pointers
			filled_lines.push_back(*iter);
		}
	}

	// make sure those lines have the same sum value

	bool all_lines_equal = true;

	if (filled_lines.size() == 0 || filled_lines.size() == 1) {
		// nothing here, continue to return
	}
	else {
		AbelianGroupElement sum = (*filled_lines[0]).get_point_sum();

		vector<Line*>::iterator filled_iter = filled_lines.begin();
		filled_iter++;

		for (filled_iter; filled_iter < filled_lines.end(); filled_iter++) {
			if ((*(*filled_iter)).get_point_sum() != sum) {
				all_lines_equal = false;
				break;
			}
		}
	}

	return all_lines_equal;


}

void assign_point(vector<Point*>& point_set, 
	vector<Line*>& line_set, 
	vector<AbelianGroupElement> element_set, 
	map<Point*, AbelianGroupElement> map, 
	FinderReturn& result) {

	// check to see if all points have been assigned
		// if true, initialize results and pass up stack

	if (point_set.size() == 0) {
		result.IsMagic = true;
		result.solution = map;
		return;
	}

	// if not all points assigned
		// find unassigned value, and assign to point

	Point* to_assign = point_set[0];

	vector<AbelianGroupElement>::iterator value_iter;

	for (value_iter = element_set.begin(); value_iter < element_set.end(); value_iter++) {

		map[to_assign] = *value_iter;

		if (filled_lines_are_invariant(line_set, map)) {
			vector<Point*> new_point_set = point_set;
			pop_front(new_point_set);

			vector<AbelianGroupElement> new_element_set = element_set;
			vector<AbelianGroupElement>::iterator position = std::find(
				new_element_set.begin(), 
				new_element_set.end(), 
				*value_iter);

			if (position != new_element_set.end()) { new_element_set.erase(position); }

			assign_point(new_point_set, line_set, new_element_set, map, result);

			if (result.IsMagic) {
				return;
			}
			else {
				// ENDS result if, ENDS line-check if, ENDS for loop iteration
			}
			
		}

	}

}

string dump_map(map<Point*, AbelianGroupElement>& mapex) {

	map<Point*, AbelianGroupElement>::iterator iter;
	string str;
	str += "[";
	for (iter = mapex.begin(); iter != mapex.end(); iter++) {
		str += iter->second.convert_to_string();
		str += ", ";
	}
	str.pop_back();
	str.pop_back();
	str += "]";
	return str;
}


