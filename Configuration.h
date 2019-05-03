#pragma once
#include "FiniteAbelianGroups.h"
#include <vector>
#include "json.hpp"

using std::vector;
using std::string;
using json = nlohmann::json;

class Line;
class Configuration;
class Point;

vector<vector<int>> get_line_matrix_from_json(json input) {

	vector<vector<int>> output_vec;

	for (json::iterator iter = input.begin(); iter != input.end(); iter++) {

		vector<int> temp;

		for (json::iterator iter2 = (*iter).begin(); iter2 != (*iter).end(); iter2++) {
			temp.push_back((*iter2));
		}

		output_vec.push_back(temp);

	}

	return output_vec;
}

class Point {
public:
	int id;
	int degree;
	AbelianGroupElement value;
	bool value_assigned = false;
	vector<Line*> lines;

	Point(int degree, int id);

	void assign_lines(vector<Line*>& to_assign);
	void assign_value(AbelianGroupElement to_assign);
	void replace_value(AbelianGroupElement to_assign);
	vector<Line*>& get_lines();

	AbelianGroupElement operator+(Point& other);
	bool operator<(Point& other);
};

Point::Point(int deg, int i) {
	degree = deg;
	id = i;
}

void Point::assign_lines(vector<Line*>& to_assign) {
	try {
		if (lines.size() == degree) {
			throw "Cannot add more lines to a point already assigned lines";
		}

		if (degree != to_assign.size()) {
			throw "Cannot add more/less lines than degree of point";
		}

		lines = to_assign;
	}

	catch (string err) {
		throw;
	}
}

void Point::assign_value(AbelianGroupElement to_assign) {
	try {
		if (value_assigned == true) {
			throw "Point already has a value";
		}

		value = to_assign;
		value_assigned = true;
	}

	catch (string err) {
		throw;
	}
}

void Point::replace_value(AbelianGroupElement to_assign) {
	try {
		if (value_assigned == false) {
			throw "Cannot replace a value when point has no value already assigned";
		}

		value = to_assign;
	}

	catch (string err) {
		throw;
	}
}

AbelianGroupElement Point::operator+(Point& other) {
	try {
		if (value_assigned == false || other.value_assigned == false) {
			throw "Cannot add points that have not yet been assigned values";
		}

		return this->value + other.value;
	}
	catch (string err) {
		throw;
	}
}

bool Point::operator<(Point& other) {
	if (this->id < other.id) { return true; }
	else { return false; }
}

vector<Line*>& Point::get_lines() {
	return lines;
}

class Line {
public:
	int degree;
	vector<Point*> points;

	Line(int degree);

	void assign_points(vector<Point*>& to_assign);
	bool is_full();

	vector<Point*>& get_points();

	AbelianGroupElement get_point_sum();
};

Line::Line(int deg) {
	degree = deg;
}

void Line::assign_points(vector<Point*>& to_assign) {
	try {
		if (points.size() == degree) {
			throw "Cannot add more points to a line already assigned points";
		}

		if (degree != to_assign.size()) {
			throw "Cannot add more/less points than degree of line";
		}

		points = to_assign;
	}

	catch (string err) {
		throw;
	}
}

vector<Point*>& Line::get_points() {
	return this->points;
}

AbelianGroupElement Line::get_point_sum() {
	vector<Point*>::iterator iter = points.begin();
	iter++;

	AbelianGroupElement sum = (*points[0]).value;
	for (iter; iter < points.end(); iter++) {
		sum = sum + (*(*iter)).value;
	}

	return sum;
}

bool Line::is_full() {
	vector<Point*>::iterator iter;

	bool points_are_full = true;

	for (iter = points.begin(); iter < points.end(); iter++) {
		if ((*iter)->value_assigned == false) {
			points_are_full = false;
			break;
		}
	}

	return points_are_full;
}

class Configuration {
private:
	vector<Line*> lines;
	vector<Point*> points;
	int degree;
	int size;

public:
	Configuration(vector<vector<int>> line_matrix);
	Configuration(json input);
	~Configuration();
	void make_points_and_lines(vector<vector<int>> line_matrix);
	int get_determinant();

	vector<Line*> get_lines() { return lines; }
	vector<Point*> get_points() { return points; }


};

void Configuration::make_points_and_lines(vector<vector<int>> line_matrix) {
	degree = line_matrix[0].size();
	size = line_matrix.size();

	// make points and give them ids
	for (int k = 0; k < size; k++) {
		Point* to_create = new Point(degree, k);
		points.push_back(to_create);
	}

	vector<vector<int>>::iterator line_iter;

	for (line_iter = line_matrix.begin(); line_iter < line_matrix.end(); line_iter++) {
		// (*line_iter) is a vector integers
		vector<Point*> points_to_assign;
		vector<int>::iterator iter;

		for (iter = (*line_iter).begin(); iter < (*line_iter).end(); iter++) {
			points_to_assign.push_back(this->points[(*iter)]);
		}

		Line* to_create = new Line(degree);

		(*to_create).assign_points(points_to_assign);

		lines.push_back(to_create);
	}
}

Configuration::Configuration(vector<vector<int>> line_matrix) {
	make_points_and_lines(line_matrix);
}

Configuration::Configuration(json input) {
	vector<vector<int>> line_matrix = get_line_matrix_from_json(input);

	make_points_and_lines(line_matrix);
}

Configuration::~Configuration() {
	// nothing to do here YET
}