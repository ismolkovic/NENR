#ifndef OPERATOR_H
#define OPERATOR_H

#include <iostream>
using namespace std;

#include "func.h"
#include "fuzzyset.h"
#include "domain.h"

// presjek
class ZadehT : public FuzzySet {
	private:
		FuzzySet **left, **right;
	public:
		ZadehT(FuzzySet**, FuzzySet**);
		~ZadehT(){ }
		double getMembership(Object);
		double operation(double, double);
		Domain *getDomain();
};

// unija
class ZadehS : public FuzzySet {
	private:
		FuzzySet **left, **right;
	public:
		ZadehS(FuzzySet**, FuzzySet**);
		~ZadehS(){ }
		double getMembership(Object);
		double operation(double, double);
		Domain *getDomain();
};

class ZadehNot : public FuzzySet {
	private:
		FuzzySet **set;
	public:
		ZadehNot(FuzzySet**);
		~ZadehNot() { }
		double getMembership(Object);
		double operation(double);
		Domain *getDomain();
};

class HammacherT : public FuzzySet {
	private:
		FuzzySet **left, **right;
		double v;
	public:
		HammacherT(double, FuzzySet**, FuzzySet**);
		~HammacherT() { }
		double getMembership(Object);
		double operation(double, double);
		Domain *getDomain();
};

class HammacherS : public FuzzySet {
	private:
		FuzzySet **left, **right;
		double v;
	public:
		HammacherS(double, FuzzySet**, FuzzySet**);
		~HammacherS() { }
		double getMembership(Object);
		double operation(double, double);
		Domain *getDomain();
};

class Mamdani : public FuzzySet {
	private:
		FuzzySet **left, **right;
		Domain *ptr;
			
	public:
		Mamdani(Domain*, FuzzySet**, FuzzySet**);
		~Mamdani(){ }

		double getMembership(Object);
		double operation(Object&, Object&);
		Domain* getDomain();
};

#endif