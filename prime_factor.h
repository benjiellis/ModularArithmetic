#pragma once
#include <cmath>
#include <vector>

using std::vector;

// A function to print all prime factors of a given number n
vector<int> get_prime_factors(int n) {

	vector<int> to_return;
	// Print the number of 2s that divide n
	while (n % 2 == 0)
	{
		to_return.push_back(2);
		n = n / 2;
	}

	// n must be odd at this point.  So we can skip 
	// one element (Note i = i +2)
	for (int i = 3; i <= sqrt(n); i = i + 2)
	{
		// While i divides n, print i and divide n
		while (n%i == 0)
		{
			to_return.push_back(i);
			n = n / i;
		}
	}

	// This condition is to handle the case when n 
	// is a prime number greater than 2
	if (n > 2)
		to_return.push_back(n);

	return to_return;
}


vector<int> store_part(vector<int>& v, int level) {
	vector<int> temp;
	for (int i = 0; i <= level; i++) {
		temp.push_back(v[i]);
	}

	return temp;
		
}

void part(int n, vector<int>& v, int level, vector<vector<int>>& out_vec) {
	int first; /* first is before last */

	if (n<1) return;
	v[level] = n;
	out_vec.push_back(store_part(v, level));

	first = (level == 0) ? 1 : v[level - 1];

	for (int i = first; i <= n / 2; i++) {
		v[level] = i; /* replace last */
		part(n - i, v, level + 1, out_vec);
	}
}

vector<vector<int>> get_partitions(int n) {
	vector<vector<int>> to_return;
	vector<int> temp(n);
	part(n, temp, 0, to_return);
	return to_return;
}