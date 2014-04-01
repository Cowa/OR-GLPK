#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <glpk.h>
#include <stdio.h>
#include "data.cpp"

using namespace std;

typedef struct {
	vector<int> way;
	int length;
} Tour;

vector<Tour>* please_enumerate(data *p, vector<Tour> *enumerate, vector<int> way, int cursor, int cap);
string please_print_vt(vector<Tour> v);
string please_print_v(vector<int> v);
bool please_is_it_terminal(vector<int> *v, int n);
Tour please_seek_minimal(data *p, vector<int> *v);
int please_compute_length(data *p, vector<int> *v);

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
	cout << please_print_vt(e) << endl;

	/**************
	 * GLPK BELOW *
	 **************/

	// Initial creation of the GLPK problem
	glp_prob *prob;
	prob = glp_create_prob();
	glp_set_prob_name(prob, "GLPK resolution of the tours");
	glp_set_obj_dir(prob, GLP_MIN);

	// Initial informations
	int nb_var = e.size();
	int nb_cont = p.n - 1; // have to check that one

	// Arrays for the constraints
	vector<int> ia;
	vector<int> ja;
	vector<double> ar;

	// A few variables
	char constr_names[nb_var+1][20];
	char var_names[nb_cont+1][20];
	double z;
	double x[nb_var+1];

	// Constraints
	glp_add_rows(prob, nb_cont);

	for (int i=1; i<=nb_cont; i++) {
		sprintf(constr_names[i], "constraint n°%d", i);
		glp_set_row_name(prob, i, constr_names[i]);
		glp_set_row_bnds(prob, i, GLP_FX, 1.0, 0.0); // Equality constraint
	}

	// Variables
	glp_add_cols(prob, nb_var);

	for (int i=1; i<=nb_var; i++) {
		sprintf(var_names[i], "x%d", i);
		glp_set_col_name(prob, i, var_names[i]);
		glp_set_col_bnds(prob, i, GLP_DB, 0.0, 1.0); // Bounds
		glp_set_col_kind(prob, i, GLP_BV); // Binary
	}

	// Objective
	for (int i=1; i<=nb_var; i++) {
		glp_set_obj_coef(prob, i, e[i-1].length);
	}

	// Matrix of the constraints
	ia.push_back(0); // Because ia, ja and ar have to start at 1
	ja.push_back(0);
	ar.push_back(0.0);
	for (int i=0; i<nb_var; i++) {
		for (unsigned int j=0; j<e[i].way.size(); j++) { // Maybe we could use a "for each"-like statement here
			ia.push_back(e[i].way[j]);
			ja.push_back(i+1);
			ar.push_back(1.0);
		}
	}

	glp_load_matrix(prob, ia.size()-1, ia.data(), ja.data(), ar.data()); // Use of data() because GLPK cannot deal with vectors
	glp_write_lp(prob, NULL, "debug.lp");

	// Resolution
	glp_simplex(prob, NULL);
	glp_intopt(prob, NULL);

	// Data recovery
	z = glp_mip_obj_val(prob); // Optimal value
	for (int i=0; i<nb_var; i++) { // Variable values
		x[i] = glp_mip_col_val(prob, i+1);
	}

	printf("z= %lf\n",z);
	for (int i=0; i<nb_var; i++) {
		printf("x%d = %d, ", i, (int)(x[i] + 0.5)); // To modify, it's just a bad copy/paste of the tp3, like a bad motherfucker
	}
	puts("");

	// Data releasing
	glp_delete_prob(prob);

	/************
	 * END GLPK *
	 ************/

	free_data(&p);

	return 0;
}

/***********************
 * THIS. IS. FUNCTIONS *
 ***********************/

// Enumerate possibilities
vector<Tour>* please_enumerate(data *p, vector<Tour> *enumerate, vector<int> way, int cursor, int cap) {

	if (please_is_it_terminal(&way, p->n - 1)) {
		//cout << "Terminating !" << endl;
		return enumerate;

	} else if (cursor > p->n - 1) {
		//cout << "Cursoring > n...." << endl;
		cursor = way.back();
		way.pop_back();
		cap -= p->d[cursor];
		//cout << "Done cursoring > n." << endl;

	} else if (p->d[cursor] + cap <= p->ca) {
		//cout << "Adding client...." << endl;
		way.push_back(cursor);
		enumerate->push_back(please_seek_minimal(p, &way));
		cap += p->d[cursor];
		//cout << "Done adding client." << endl;

	} return please_enumerate(p, enumerate, way, cursor + 1, cap);
}

// Return the permutation with minimal length
Tour please_seek_minimal(data *p, vector<int> *v) {

	int min = 9999999, tmp = 0;
	Tour t;
	
	//cout << "Seeking minimal..." << endl;
	
	do {
		tmp = please_compute_length(p, v);
		if (tmp < min) {
			min = tmp;
			t.way = *v;
			t.length = min;
		}
	} while (next_permutation(v->begin(),v->end()));
	
	//cout << "Seeking done." << endl;
	
	return t;
}

// Compute length of a way
int please_compute_length(data *p, vector<int> *v) {
	
	//cout << "Computing length..." << endl;
	int len = p->C[0][v->at(0)];

	for (unsigned int i = 1; i < v->size(); i++) {
		len += p->C[v->at(i-1)][v->at(i)];
	}

	len += p->C[v->back()][0];
	
	//cout << "Computing done." << endl;
	return len;
}

// Check the terminal state
bool please_is_it_terminal(vector<int> *v, int n) {

	cout << "Checking terminating..." << endl;

	if (v->size() != 0) {

		cout << "Terminating "<< ((bool) v->at(0) == n) << endl;
		return v->front() == n;
	}
	cout << "Terminating 'cause of size 0." << endl;
	return false;
}

// Create a pretty string for vector of Tour
string please_print_vt(vector<Tour> vt) {

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

