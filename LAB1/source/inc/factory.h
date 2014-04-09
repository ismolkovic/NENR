#ifndef FACTORY_H
#define FACTORY_H 

#include <iostream>
using namespace std;
#include "app.h"


// sadrži factory metode
// 
namespace Factory {
	
	// factory metode
	Domain* domainFactory(string,string);
	FuzzySet* createSimpleSet(string, vector< string >&);
	FuzzySet* fuzzySetFactory(string, string, vector< string>&);
	FuzzySet** operatorFactory(string, FuzzySet**, FuzzySet**);
	FuzzySet* createExpr(FuzzySet**);
	FuzzySet* createProject(FuzzySet*, Domain*);
	FuzzySet* createCylindricalExtension(FuzzySet*, Domain*);
	FuzzySet* createComposition(FuzzySet*, FuzzySet*, string);
	Domain* createCompositionDomain(string);


	// pomocna metoda 
	string createCompositionDomainString(FuzzySet*, FuzzySet*, vector<int>&);
}

#endif