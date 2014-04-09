#ifndef GRADIENT_HEADER
#define GRADIENT_HEADER

#include "system.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <time.h>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

class BatchGD {
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

public:
	BatchGD(int, Data*);
	~BatchGD();
	void run(double eps, int iter, double eta);
	void print();
};


#endif