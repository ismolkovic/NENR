#ifndef FUZZY_H
#define FUZZY_H 

#include <iostream>
using namespace std;

#include "domain.h"
#include "func.h"

class FuzzySet {
	private:
	public:
		virtual Domain* getDomain()=0;
		virtual double getMembership(Object)=0;
		virtual double getMembership() { return 0; }

		void write();
		FuzzySet() { }
		virtual ~FuzzySet() { }
};

class SimpleSet : public FuzzySet {
	private:
		Domain *domainPointer;
		map< int, double > membership;
		void insertMembership(int, double);

	public:
		Domain* getDomain();
		SimpleSet(Domain*,vector<PAIRS>);
		SimpleSet(Domain*, map<int,double>&);
		~SimpleSet(){ }
		double getMembership(Object);
		double getMembershipAt(int);
};


class Gamma : public FuzzySet {
	private:
		Domain *domainPointer;
		map< int, double > membership;
		Object alpha, beta;

	public:
		Gamma(Domain*, Object, Object);
		~Gamma() { }
		double getMembership(Object);
		double getMembershipAt(int);
		Domain* getDomain();
};

class Lambda : public FuzzySet {
	private:
		Domain *domainPointer;
		map< int, double > membership;
		Object alpha, beta, gamma;

	public:
		Lambda(Domain*, Object, Object, Object);
		~Lambda() {  }
		double getMembership(Object);
		double getMembershipAt(int);
		Domain* getDomain();
};

class L : public FuzzySet {
	private:
		Domain *domainPointer;
		map< int, double > membership;
		Object alpha, beta;

	public:
		L(Domain*, Object, Object);
		~L() {  }
		double getMembership(Object);
		double getMembershipAt(int);
		Domain* getDomain();
};

class Pi : public FuzzySet {
	private:
		Domain *domainPointer;
		map< int, double > membership;
		Object alpha, beta, gamma, delta;

	public:
		Pi(Domain*, Object, Object, Object, Object);
		~Pi() { }
		double getMembership(Object);
		double getMembershipAt(int);
		Domain* getDomain();
};


class Expr : public FuzzySet {
	private:
		Domain *domainPointer;
		FuzzySet **ptr; // pokazivac na stablo

	public:
		Expr(Domain*, FuzzySet**);
		~Expr() { }
		double getMembership(Object);
		Domain* getDomain();
};


#endif