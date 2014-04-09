#include "control.h"


// class MinimumEngine

MinimumEngine::MinimumEngine(Rules *r, Decoder *d) : rules(r), decoder(d) { }

double MinimumEngine::getOutput(vector<Object>& input) {
	map< int, double > ByMembership; // rezultat 
	Domain *d = NULL; 
	// idemo kroz sva pravila 

	for(int i = 0; i < rules->getNumberOfRules(); i++) {
		// napravimo kodiranje ulaza u singletone 
		vector< double > miA = convertToMembership(input, i);

		if(miA.size() == 0) throw 40;


		double minMembershipA = *std::min_element(miA.begin(), miA.end());

		//cout << minMembershipA << endl;

		FuzzySet *Biy = rules->getConsequent(i);
		
		if (d == NULL) d = Biy->getDomain();

		for(int y = 0; y < Biy->getDomain()->getCardinality(); y++) {
			Object t = Biy->getDomain()->elementAt(y);
			double mem = Biy->getMembership(t);
		
			double value = min(minMembershipA, mem);

			ByMembership[ y ] = max( value, ByMembership[ y ]);
		}
	}

	// neizraziti skup nad izlazom
	FuzzySet *B = new SimpleSet(d, ByMembership); 	
	double ret = decoder->calculate(B);

	//B->write();

	if (B != NULL ) delete B;

	return ret;
}

vector< double > MinimumEngine::convertToMembership(vector<Object>& v, int r) {
	vector< double > ret;
	map< int, FuzzySet* >& antecedent = rules->getAntecedent(r);

	for(int i = 0; i < (int)v.size(); i++) {
		if ( antecedent.find(i) != antecedent.end()) {
			ret.push_back(antecedent.at(i)->getMembership(v.at(i)));
		}
	}

	return ret;
}

// class ProductEngine

ProductEngine::ProductEngine(Rules *r, Decoder *d) : rules(r), decoder(d) { }


double ProductEngine::getOutput(vector<Object>& input) {
	map< int, double > ByMembership; // rezultat 
	Domain *d = NULL; 
	// idemo kroz sva pravila 

	for(int i = 0; i < rules->getNumberOfRules(); i++) {
		// napravimo kodiranje ulaza u singletone 
		vector< double > miA = convertToMembership(input, i);

		if(miA.size() == 0) throw 40;
		double MembershipA = 1;
		for(int i = 0; i < (int)miA.size(); i++) MembershipA *= miA.at(i);


		FuzzySet *Biy = rules->getConsequent(i);
		
		if (d == NULL) d = Biy->getDomain();

		for(int y = 0; y < Biy->getDomain()->getCardinality(); y++) {
			Object t = Biy->getDomain()->elementAt(y);
			double mem = Biy->getMembership(t);
		
			double value = MembershipA * mem;

			ByMembership[ y ] = max( value, ByMembership[ y ]);
		}
	}

	// neizraziti skup nad izlazom
	FuzzySet *B = new SimpleSet(d, ByMembership); 
	double ret = decoder->calculate(B);

	if (B != NULL ) delete B;

	return ret;
}

vector< double > ProductEngine::convertToMembership(vector<Object>& v, int r) {
	vector< double > ret;
	map< int, FuzzySet* >& antecedent = rules->getAntecedent(r);

	for(int i = 0; i < (int)v.size(); i++) {
		if ( antecedent.find(i) != antecedent.end()) {
			ret.push_back(antecedent.at(i)->getMembership(v.at(i)));
		}
	}

	return ret;
}

// defuzzyficaation
// CenterOfArea
double CenterOfArea::calculate(FuzzySet *s) {
	if (s == NULL) throw 40;

	double m1 = 0, m0 = 0;
	for(unsigned int i = 0; i < s->getDomain()->getCardinality(); i++) {
		Object a = s->getDomain()->elementAt(i);
		
		double memb = s->getMembership(a);

		m1 += memb * a;
		m0 += memb;
	}

	if((int)m1 == 0) return 0;

	return m1 / m0;
}