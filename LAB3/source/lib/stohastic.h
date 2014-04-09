#ifndef STOHASTIC_HEADER
#define STOHASTIC_HEADER

#include "system.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

using namespace std;

class StohasticGD {
private:
	vector< double > p, q, r, a, b, c;
	vector< double > w, f, miA, miB;
	int N;

	Data *input;
	Output *outPtr;
	FuzzySet *fuzzyPtr;
	Antecedent *antPtr;
	Consequent *conPtr;
	Derivation *derPtr;

	vector<double> generateRandom(int);
	void generateWF(double&, double&, double&);
	void updateParams(double&, double&, double&, double&, double&);

public:
	StohasticGD(int, Data*);
	~StohasticGD();
	void run(double eps, int iter, double eta);
	void print();
};


#endif