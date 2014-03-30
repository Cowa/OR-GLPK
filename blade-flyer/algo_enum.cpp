/* Compile it this way: g++ -std=c++0x algo_enum.cpp -o algo_enum */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "data.cpp"

using namespace std;

typedef struct {
	vector<int> way;
	int length;
} Tour;

vector<Tour>* please_enumerate(data *p, vector<Tour> *enumerate, vector<int> way, int cursor, int cap);
string please_print_vv(vector<Tour> v);
string please_print_v(vector<int> v);
bool please_is_it_terminal(vector<int> v, int n);
Tour please_seek_minimal(data *p, vector<int> v);
int please_compute_length(data *p, vector<int> v);

/******************
 * THIS. IS. MAIN *
 ******************/

// HOW TO LAUNCH :
// Launch it with an argument to the data file, ex: ./algo_enum A/VRPA35.dat
int main(int argc, char *argv[]) {

	data p;
	vector<Tour> e;

	read_data(argv[1],&p);

	please_enumerate(&p, &e, vector<int>(), 1, 0);
	cout << please_print_vv(e) << endl;

	free_data(&p);

	return 0;
}

/***********************
 * THIS. IS. FUNCTIONS *
 ***********************/

// Enumerate possibilities
vector<Tour>* please_enumerate(data *p, vector<Tour> *enumerate, vector<int> way, int cursor, int cap) {

	if (please_is_it_terminal(way, p->n - 1)) {
		return enumerate;

	} else if (cursor > p->n - 1) {
		int pop = way.back();
		way.pop_back();
		return please_enumerate(p, enumerate, way, pop+1, cap - p->d[pop]);

	} else if (p->d[cursor] + cap <= p->ca) {
		way.push_back(cursor);
		enumerate->push_back(please_seek_minimal(p, way));
		return please_enumerate(p, enumerate, way, cursor + 1, cap + p->d[cursor]);

	} return please_enumerate(p, enumerate, way, cursor + 1, cap);
}

// Return the permutation with minimal length
Tour please_seek_minimal(data *p, vector<int> v) {

	int min = 9999999, tmp = 0;
	Tour t;

	do {
		tmp = please_compute_length(p, v);
		if (tmp < min) {
			min = tmp;
			t.way = v;
			t.length = min;
		}
	} while (next_permutation(v.begin(),v.end()));

	return t;
}

// Compute length of a way
int please_compute_length(data *p, vector<int> v) {

	int len = p->C[0][v[0]];

	for (int i = 1; i < v.size(); i++) {
		len += p->C[v[i-1]][v[i]];
	}

	len += p->C[v.back()][0];

	return len;
}

// Check the terminal state
bool please_is_it_terminal(vector<int> v, int n) {

	if (v.size() != 0) {
		return v[0] == n;
	}
	return false;
}

// Create a pretty string for vector of Tour
string please_print_vv(vector<Tour> vt) {

	string print;

	for (Tour & t : vt) {
		print += to_string(t.length) + " ";
		print += please_print_v(t.way) + "\n";
	}
	return print;
}

// Create a pretty string for vector
string please_print_v(vector<int> v) {

	string print;
	
	print += "[ ";
	for (int & i : v) {
		print += to_string(i) + " ";
	}
	print += "]";
	return print;
}
