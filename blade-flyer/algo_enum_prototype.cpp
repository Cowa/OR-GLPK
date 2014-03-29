/* Compile it this way: g++ -std=c++0x algo_enum.cpp -o algo_enum */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef struct Tour Tour;
struct Tour{
	vector<int> way;
	int length;
};

vector<Tour>* please_enumerate(vector<Tour> *enumerate, vector<int> way, int cursor, int cap);
string please_print_vv(vector<Tour> v);
string please_print_v(vector<int> v);
bool please_is_it_terminal(vector<int> v, int n);
Tour please_seek_minimal(vector<int> v);
int please_compute_length(vector<int> v);

int n = 5;
int ca = 10;
int d[6] = {-1, 2, 4, 2, 4, 2};
int c[6][6] = {{0,334,262,248,277,302},{334,0,118,103,551,105},{262,118,0,31,517,180},{248,103,31,0,495,152},{277,551,517,495,0,476},{302,105,180,152,476,0}};

/******************
 * THIS. IS. MAIN * <--- Here's the main
 ******************/

int main() {

	vector<Tour> e;
	
	please_enumerate(&e, vector<int>(), 1, 0);
	cout << please_print_vv(e) << endl;
	return 0;
}

/***********************
 * THIS. IS. FUNCTIONS *
 ***********************/

// Enumerate possibilities
vector<Tour>* please_enumerate(vector<Tour> *enumerate, vector<int> way, int cursor, int cap) {

	if (please_is_it_terminal(way, n)) {
		return enumerate;

	} else if (cursor > n) {
		int pop = way.back();
		way.pop_back();
		return please_enumerate(enumerate, way, pop+1, cap - d[pop]);

	} else if (d[cursor] + cap <= ca) {
		way.push_back(cursor);
		enumerate->push_back(please_seek_minimal(way));
		return please_enumerate(enumerate, way, cursor + 1, cap + d[cursor]);

	} else {
		return please_enumerate(enumerate, way, cursor + 1, cap);
	}
}

// Return the permutation with minimal length
Tour please_seek_minimal(vector<int> v) {

	int min = 9999, tmp = 0;
	Tour t;

	do {
		tmp = please_compute_length(v);
		if (tmp < min) {
			min = tmp;
			t.way = v;
			t.length = min;
		}
	} while (next_permutation(v.begin(),v.end()));

	return t;
}

// Compute length of a way
int please_compute_length(vector<int> v) {

	int len = 0;

	for (int i = 0; i < v.size(); i++) {
		if (i == 0)
			len += c[0][v[i]];
		else
			len += c[v[i-1]][v[i]];
	}

	len += c[v.back()][0];

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
