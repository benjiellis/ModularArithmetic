#pragma once

#include <vector>
#include <iterator>

// credit to user https://stackoverflow.com/users/13895/john

using std::vector;

template <typename T>
struct Digits {
	typename vector<T>::iterator begin;
	typename vector<T>::iterator end;
	typename vector<T>::iterator me;
};

template <typename T>
void cartesian_product(
	vector<vector<T>>& out,  // final result
	vector<vector<T>>& in)  // input result
{
	vector<Digits<T>> vd;

	// Start all of the iterators at the beginning.
	for (vector<vector<T>>::iterator it = in.begin();
	it != in.end();
		++it) {
		Digits<T> d = { (*it).begin(), (*it).end(), (*it).begin() };
		vd.push_back(d);
	}


	while (1) {

		// Construct your first product vector by pulling 
		// out the element of each vector via the iterator.
		vector<T> result;
		for (vector<Digits<T>>::const_iterator it = vd.begin();
		it != vd.end();
			it++) {
			result.push_back(*(it->me));
		}
		out.push_back(result);

		// Increment the leftmost one, and repeat.

		// When you reach the end, reset that one to the beginning and
		// increment the next one. You can get the "next"
		// iterator by pulling it out of the neighboring element in your
		// vector of iterators.
		for (vector<Digits<T>>::iterator iter = vd.begin(); ; ) {
			// okay, I started at the left instead. sue me
			++(iter->me);
			if (iter->me == iter->end) {
				if (iter + 1 == vd.end()) {
					// I'm the last digit, and I'm about to roll
					return;
				}
				else {
					// cascade
					iter->me = iter->begin;
					++iter;
				}
			}
			else {
				// normal
				break;
			}
		}
	}
}