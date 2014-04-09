#include "operator.h"

// ZadehT presjek
ZadehT::ZadehT(FuzzySet **f, FuzzySet **s) : 
		left(f), right(s) {
}

double ZadehT::getMembership(Object o) {
	double val1 = (*left)->getMembership(o);
	double val2 = (*right)->getMembership(o);

	return operation(val1, val2);
}

double ZadehT::operation(double f, double s) {
	return min(f, s);
}

Domain* ZadehT::getDomain() { return (*left)->getDomain(); }

// ZadehS unija
ZadehS::ZadehS(FuzzySet **f, FuzzySet **s) : 
		left(f), right(s) {
}

double ZadehS::getMembership(Object o) {
	double val1 = (*left)->getMembership(o);
	double val2 = (*right)->getMembership(o);

	return operation(val1, val2);
}

double ZadehS::operation(double f, double s) {
	return max(f, s);
}


Domain* ZadehS::getDomain() { return (*left)->getDomain(); }

// ZadehNot
ZadehNot::ZadehNot(FuzzySet **s) : set(s) {
}

double ZadehNot::getMembership(Object o) {
	double val = (*set)->getMembership(o);

	return operation(val);
}

double ZadehNot::operation(double v) {
	return (1 - v);
}


Domain* ZadehNot::getDomain() { return (*set)->getDomain(); }

// HammacherT
HammacherT::HammacherT(double _v, FuzzySet **f, FuzzySet **s) :  
	left(f), right(s), v(_v) {
}

double HammacherT::getMembership(Object o) {
	double val1 = (*left)->getMembership(o);
	double val2 = (*right)->getMembership(o);

	return operation(val1, val2);
}

double HammacherT::operation(double a, double b) {
	return (a * b) / (v +(1-v) * (a + b - a * b));
}


Domain* HammacherT::getDomain() { return (*left)->getDomain(); }

// HammacherS
HammacherS::HammacherS(double _v, FuzzySet **f, FuzzySet **s) :  
	left(f), right(s), v(_v) {
}

double HammacherS::getMembership(Object o) {
	double val1 = (*left)->getMembership(o);
	double val2 = (*right)->getMembership(o);

	return operation(val1, val2);
}

double HammacherS::operation(double a, double b) {
	return (a + b -(2-v) * (a * b)) / ( 1 - (1-v) * a * b) ;
}

Domain* HammacherS::getDomain() { return (*left)->getDomain(); }


// Mamdani operator
Mamdani::Mamdani(Domain *d, FuzzySet **f, FuzzySet **s) :  
	left(f), right(s), ptr(d) {
}

Domain* Mamdani::getDomain() {
	if(ptr == NULL) throw 10;
	return ptr;
}

// ocekujemo objekt koji je tipa Object(a,b)
double Mamdani::getMembership(Object o) {
	vector< Object > o_ = Object2vectorObject(o);
	vector< Object > a, b;
	// tu napravimo split
	int N = (*left)->getDomain()->getDomainComponents().size();
	
	for(int i = 0; i < N; i++) a.push_back(o_.at(i));
	for(int i = N; i < (int)o_.size(); i++) b.push_back(o_.at(i));

	Object aret = vObject2Object(a);
	Object bret = vObject2Object(b);

	return operation(aret, bret);
}

double Mamdani::operation(Object& a, Object& b) {
	double val1 = (*left)->getMembership(a);
	double val2 = (*right)->getMembership(b);

	return min(val1, val2);
}