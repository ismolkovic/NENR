#ifndef TEST_H
#define TEST_H 

#include <iostream>
using namespace std;

#include "app.h"

namespace Test {
	bool testCartesianSelf(Domain*);

	bool testSymmetric(FuzzySet*);
	bool testReflexive(FuzzySet*);
	bool testTransitive(FuzzySet*, string);

	bool testMaxMin(FuzzySet*);
	bool testMaxProduct(FuzzySet*);

	bool testFuzzyEquivalence(FuzzySet*, string);
}


#endif