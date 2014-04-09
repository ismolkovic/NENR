#ifndef RULES_H
#define RULES_H 

#include "control.h"

/*
		Baza ako onda pravila
*/

class Rules { 
	private:
		int Nr;
		map< int, map< int, FuzzySet* > > mapAntecedent;
		map< int, FuzzySet* > mapConsequent;

		void insertRule(map< int, FuzzySet* >&, FuzzySet*);

	public:
		map< int, FuzzySet* >& getAntecedent(int); 
		FuzzySet* getConsequent(int);

		Rules() : Nr(0) { }

		long getNumberOfRules(); // vraca broj pravila
		void initKormilo();
		void initAkceleracija();

		~Rules();
};


#endif