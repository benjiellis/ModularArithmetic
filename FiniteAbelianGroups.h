#pragma once

#include <vector>
#include "cartesian_product.h"
#include "prime_factor.h"
#include <set>
#include <algorithm>
#include <sstream>
#include <cmath>

using std::vector;
using std::set;
using std::string;
using std::ostringstream;
using std::ostream;
typedef vector<vector<vector<int>>> super_vec;

const int zero = 0;

template<typename T>
std::vector<T> flatten(const std::vector<std::vector<T>> &orig)
{
	std::vector<T> ret;
	for (const auto &v : orig)
		ret.insert(ret.end(), v.begin(), v.end());
	return ret;
}

class AbelianGroupElement {
private:
	vector<int> values;
	vector<int> moduli;

public:
	AbelianGroupElement();
	AbelianGroupElement(vector<int>& val, vector<int>& mod);
	AbelianGroupElement(vector<int>& mod);
	~AbelianGroupElement();
	// AbelianGroupElement(AbelianGroupElement& elem);

	AbelianGroupElement add_to(AbelianGroupElement& right);
	AbelianGroupElement subtract_from(AbelianGroupElement& right);

	bool operator==(const AbelianGroupElement& other);
	AbelianGroupElement operator+(AbelianGroupElement& other);
	AbelianGroupElement operator-(AbelianGroupElement& other);
	bool operator!=(AbelianGroupElement& other);
	friend ostream& operator<<(ostream& os, const AbelianGroupElement& to_print);
	string convert_to_string();

	AbelianGroupElement get_inverse();
	vector<AbelianGroupElement> get_full_group();
	vector<int>& get_values();
	vector<int>& get_moduli();
};

AbelianGroupElement::AbelianGroupElement() {
	values.push_back(zero);
	moduli.push_back(zero);
}

AbelianGroupElement::AbelianGroupElement(vector<int>& val, vector<int>& mod) {
	values = val;
	moduli = mod;
}

AbelianGroupElement::AbelianGroupElement(vector<int>& mod) {
	moduli = mod;
	values = vector<int>(mod.size(), 0);
}

//AbelianGroupElement::AbelianGroupElement(AbelianGroupElement& elem) {
//	values = elem.get_values();
//	moduli = elem.get_moduli();
//}

AbelianGroupElement::~AbelianGroupElement() {
	// nothing here
}

vector<int>& AbelianGroupElement::get_values() {
	return this->values;
}

vector<int>& AbelianGroupElement::get_moduli() {
	return this->moduli;
}

AbelianGroupElement AbelianGroupElement::add_to(AbelianGroupElement& right) {
	vector<int> new_values;
	vector<int> new_moduli = this->moduli;

	vector<int>::iterator iter1 = this->get_values().begin();
	vector<int>::iterator iter2 = right.get_values().begin();
	vector<int>::iterator mods = this->get_moduli().begin();

	for (iter1; iter1 < get_values().end(); iter1++, iter2++, mods++) {
		int val1 = *iter1;
		int val2 = *iter2;
		int mod = *mods;

		new_values.push_back((val1 + val2) % mod);
	}

	return AbelianGroupElement(new_values, new_moduli);
}

AbelianGroupElement AbelianGroupElement::subtract_from(AbelianGroupElement& right) {
	return this->add_to(right.get_inverse());
}

AbelianGroupElement AbelianGroupElement::operator+(AbelianGroupElement& other) {
	return this->add_to(other);
}

AbelianGroupElement AbelianGroupElement::operator-(AbelianGroupElement& other) {
	return this->subtract_from(other);
}

bool AbelianGroupElement::operator!=(AbelianGroupElement& other) {
	return !(*this == other);
}

AbelianGroupElement AbelianGroupElement::get_inverse() {
	vector<int> new_values;
	vector<int>::iterator iter;

	for (iter = get_values().begin(); iter < get_values().end(); iter++) {
		new_values.push_back(-(*iter));
	}

	return AbelianGroupElement(new_values, get_moduli());
}

bool AbelianGroupElement::operator==(const AbelianGroupElement& other) {
	bool cond1 = (this->values == other.values);
	bool cond2 = (this->moduli == other.moduli);

	bool is_equal = true;

	if (!cond1 || !cond2) {
		is_equal = false;
	}
	
	return is_equal;
}

vector<AbelianGroupElement> AbelianGroupElement::get_full_group() {
	// make a list for each element in mod, 0->n-1
	int count = get_moduli().size();
	vector<vector<int>> all_lists;
	vector<AbelianGroupElement> to_return;

	vector<int>::iterator iter;

	for (iter = get_moduli().begin(); iter != get_moduli().end(); iter++) {
		vector<int> temp;
		for (int i = 0; i < *iter; i++) {
			temp.push_back(i);
		}
		all_lists.push_back(temp);
	}

	vector<vector<int>> all_coefficients;

	// populates second argument with cartesian product of first
	cartesian_product(all_coefficients, all_lists);

	vector<vector<int>>::iterator iter_list;

	for (iter_list = all_coefficients.begin(); iter_list != all_coefficients.end(); iter_list++) {
		AbelianGroupElement temp(*iter_list, get_moduli());
		to_return.push_back(temp);
	}

	return to_return;
}

vector<AbelianGroupElement> get_all_groups_of_order(int order) {

	vector<int> factors = get_prime_factors(order);
	set<int> factor_set;
	vector<int>::iterator iter;
	// create set with unique factors
	for (iter = factors.begin(); iter != factors.end(); iter++) {
		factor_set.insert(*iter);
	}

	vector<AbelianGroupElement> to_return;

	set<int>::iterator set_iter;

	super_vec all_factor_combos;
	super_vec all_combos_cart_prod;

	// create all combonations of each factor by multiplicity
	for (set_iter = factor_set.begin(); set_iter != factor_set.end(); set_iter++) {
		int cnt = count(factors.begin(), factors.end(), *set_iter);

		vector<vector<int>> partitions = get_partitions(cnt);
		vector<vector<int>>::iterator part_iter;
		vector<vector<int>> multiplied_partitions; // will store each parition after being multiplied by factor

		for (part_iter = partitions.begin(); part_iter != partitions.end(); part_iter++) {
			// for each partition, multiply by the current factor
			vector<int>::iterator iter2;
			vector<int> temp;
			for (iter2 = (*part_iter).begin(); iter2 != (*part_iter).end(); iter2++) {
				temp.push_back(pow(*set_iter, *iter2));
			}
			multiplied_partitions.push_back(temp);
		}

		all_factor_combos.push_back(multiplied_partitions);
	}

	// we now have a vector of each parition for each factor
	// we need to take the cartesian products of super_vec, and then flatten the results

	cartesian_product(all_combos_cart_prod, all_factor_combos);

	// we need to flatten all_combos_cart_prod, current in form: [[2, 2, 4], [3], [5, 5]]

	super_vec::iterator iter3;
	vector<vector<int>> final_mod_list;

	for (iter3 = all_combos_cart_prod.begin(); iter3 != all_combos_cart_prod.end(); iter3++) {
		final_mod_list.push_back(flatten(*iter3));
	}

	vector<AbelianGroupElement> final_return;

	vector<vector<int>>::iterator iter4;

	for (iter4 = final_mod_list.begin(); iter4 != final_mod_list.end(); iter4++) {
		AbelianGroupElement temp(*iter4);
		final_return.push_back(temp);
	}

	return final_return;
}

ostream& operator<<(ostream& os, AbelianGroupElement& to_print) {
	os << to_print.convert_to_string();
	return os;
}

string AbelianGroupElement::convert_to_string() {
	ostringstream oss;

	vector<int>::iterator iter;

	oss << "(";

	for (iter = this->get_values().begin(); iter != this->get_values().end(); iter++) {
		oss << *iter << ", ";
	}
	// remove extra ,
	string str = oss.str();
	str.pop_back();
	str.pop_back();
	str += ")";

	return str;
}