#include "fuzzyset.h"


void FuzzySet::write() {
	Domain* d = getDomain();

	cout << "{ ";
	int N = d->getCardinality();
	bool first = true;
	for(int i = 0; i < N; i++) {
		Object o = d->elementAt(i);
		double memb = getMembership(o);

		if( memb > 0 && first) {
		cout << memb << "/" << o;
		first = false;
		}
		else if(memb > 0) cout << " + " << memb << "/" << o;
	}
	cout << " }" << endl;
}


Domain* SimpleSet::getDomain() { 
	if (domainPointer != NULL) return domainPointer;
	throw 12;
}

SimpleSet::SimpleSet(Domain *d, vector<PAIRS> input) : domainPointer(d) {

	for(vector< PAIRS >::iterator i = input.begin();
		i != input.end(); i++) 
	{
		int index = getDomain()->getIndexOfElement(getDomain()->fromStringRepresentation(i->second));
		double value = string2double(i->first);
		insertMembership(index, value);
	}
}

SimpleSet::SimpleSet(Domain *d, map<int,double>& values) : domainPointer(d) {
	for(map<int,double>::iterator i = values.begin(); i != values.end(); i++) {
		insertMembership(i->first, i->second);
	}	
}


void SimpleSet::insertMembership(int index, double value) {
	membership[ index ] = value;
}

double SimpleSet::getMembership(Object o) {
	int index = getDomain()->getIndexOfElement(o);
	return getMembershipAt(index);
}

double SimpleSet::getMembershipAt(int index) {
	if (membership.find(index) != membership.end()) {
		return membership[ index ];
	}
	return 0;
}

// Gamma
Gamma::Gamma(Domain *d, Object a, Object b) : domainPointer(d), alpha(a), beta(b) {
	for(int i = 0; i < d->getCardinality(); i++) {
		Object t = d->elementAt(i);
		double val = 0;

		if(t < alpha ) val = 0;
		if(t >= alpha && t < beta ) {val = (t - alpha ) / (beta - alpha ); }
		if ( t >= beta ) val = 1;

		membership[ i ] = val;
	}
}

Domain* Gamma::getDomain() { 
	if ( domainPointer != NULL ) return domainPointer;
	throw 20;
}

double Gamma::getMembership(Object o) {
	int index = getDomain()->getIndexOfElement(o);
	return getMembershipAt(index);
}

double Gamma::getMembershipAt(int index) {
	if (membership.find(index) != membership.end()) {
		return membership[ index ];
	}
	return 0;
}


// Lambda
Lambda::Lambda(Domain *d, Object a, Object b, Object c) : domainPointer(d), 
	alpha(a), beta(b), gamma(c) 
{
	for(int i = 0; i < d->getCardinality(); i++) {
		Object t = d->elementAt(i);
		double val = 0;

		if( t < alpha) val = 0;
		if( t >= alpha && t < beta ) { val = (t - alpha) / (beta - alpha); }
		if (t >= beta && t < gamma ) { val = (gamma - t) / (gamma - beta) ;}
		if (t >= gamma ) val = 0;

		membership[ i ] = val;
	}
}
		
double Lambda::getMembership(Object o ) {
	int index = getDomain()->getIndexOfElement(o);
	return getMembershipAt(index);
}

double Lambda::getMembershipAt(int index) {
	if (membership.find(index) != membership.end()) {
		return membership[ index ];
	}
	return 0;
}

Domain* Lambda::getDomain() {
	if ( domainPointer != NULL ) return domainPointer;
	throw 20;
}


// L
L::L(Domain *d, Object a, Object b) : domainPointer(d), alpha(a), beta(b) {

	for(int i = 0; i < d->getCardinality(); i++) {
		Object t = d->elementAt(i);
		double val = 0;

		if(t < alpha ) val = 1;
		if(t >= alpha && t < beta ) {val = (beta - t) / (beta - alpha); }
		if ( t >= beta ) val = 0;
	
		membership[ i ] = val;
	}
}

double L::getMembership(Object o) {
	int index = getDomain()->getIndexOfElement(o);
	return getMembershipAt(index);
}

double L::getMembershipAt(int index) {
	if (membership.find(index) != membership.end()) {
		return membership[ index ];
	}
	return 0;
}

Domain* L::getDomain() {
	if ( domainPointer != NULL ) return domainPointer;
	throw 20;
}

// Pi
Pi::Pi(Domain *dom, Object a, Object b, Object c, Object d) : domainPointer(dom),
	alpha(a), beta(b), gamma(c), delta(d)
{
	for(int i = 0; i < dom->getCardinality(); i++) {
		Object t = dom->elementAt(i);
		double val = 0;

		if (t < alpha) val = 0;
		if (t >= alpha && t < beta) val = (t - alpha) / (beta - alpha);
		if (t >= beta && t < gamma) val = 1;
		if (t >= gamma && t < delta) val = (delta - t) / (delta - gamma);
		if (t >= delta) val = 0;

		membership[ i ] = val;
	}
}

double Pi::getMembership(Object o) { 
	int index = getDomain()->getIndexOfElement(o);
	return getMembershipAt(index);
}

double Pi::getMembershipAt(int index) {
	if (membership.find(index) != membership.end()) {
		return membership[ index ];
	}
	return 0;
}

Domain* Pi::getDomain() {
	if ( domainPointer != NULL ) return domainPointer;
	throw 20;
}


// Expr
Expr::Expr(Domain *d, FuzzySet **p) : domainPointer(d), ptr(p) { 
}

Domain* Expr::getDomain() {
	if ( domainPointer != NULL ) return domainPointer;
	throw 20;
}

double Expr::getMembership(Object o) {
	if(ptr != NULL) {
		return (*ptr)->getMembership(o);
	}
	throw 20;
}
