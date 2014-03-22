/* Compile it this way: g++ -std=c++0x algo_enum.cpp -o algo_enum */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<vector<int> >* please_enumerate(vector<vector<int> > *enumerate, vector<int> way, int cursor, int cap);
string please_print_vv(vector<vector<int> > v);
bool please_is_it_terminal(vector<int> v, int n);

int n = 5;
int ca = 10;
int d[6] = {-1, 2, 4, 2, 4, 2};
int c[6][6] = {{0,334,262,248,277,302},{334,0,118,103,551,105},{262,118,0,31,517,180},{248,103,31,0,495,152},{277,551,517,495,0,476},{302,105,180,152,476,0}};

/******************
 * THIS. IS. MAIN * <--- Here's the main
 ******************/

int main() {

	vector<vector<int> > e;
	
	please_enumerate(&e, vector<int>(), 1, 0);
	cout << please_print_vv(e) << endl;
}

/***********************
 * THIS. IS. FUNCTIONS *
 ***********************/

// Enumerate possibilities
vector<vector<int> >* please_enumerate(vector<vector<int> > *enumerate, vector<int> way, int cursor, int cap) {

	if (please_is_it_terminal(way, n)) {
		return enumerate;

	} else if (cursor > n) {
		int pop = way.back();
		way.pop_back();
		please_enumerate(enumerate, way, pop+1, cap - d[pop]);

	} else if (d[cursor] + cap <= ca) {
		way.push_back(cursor);
		enumerate->push_back(way);
		please_enumerate(enumerate, way, cursor + 1, cap + d[cursor]);

	} else {
		please_enumerate(enumerate, way, cursor + 1, cap);
	}
}

// Create a pretty string for vector of vector
string please_print_vv(vector<vector<int> > grid) {
	
	string print;

	for (vector<int> & row : grid) {
		print += "|";
		for (int & elt : row) {
			print += " " + to_string(elt) + " ";
		}
		print += "|\n";
	}
	return print;
}

// Check the terminal state
bool please_is_it_terminal(vector<int> v, int n) {

	if (v.size() != 0) {
		return v[0] == n;
	}
	return false;
}
