#ifndef EXTENSION_H
#define EXTENSION_H 

#include <iostream> 
using namespace std;

#include "fuzzyset.h"
#include "app.h"


class Project : public FuzzySet {
	private:
		Domain *ptrDomain;
		map< int, double > membership;

		void insertMembership(int, double);
		void projectElement(Object, double);
		Object createDomainElement(Object, vector<int>&);

	public:
		// skup koji se projicira, domena na koju ide i pozicije na kojima se 
		// maksimizira
		Project(FuzzySet*, Domain*, vector<int>&);
		double getMembership(Object);
		double getMembershipAt(int);
		Domain* getDomain();
};


class CylindricalExtension : public FuzzySet {
	private:
		Domain *ptrDomain;
		map< int, double > membership;

		void ExtendElement(Object, vector<int>&, double, int);
		void insertElement(Object, double);

	public:
		CylindricalExtension(FuzzySet*, Domain*, vector<int>&);
		double getMembership(Object);
		double getMembershipAt(int);
		Domain* getDomain();

};


class Composition : public FuzzySet {
	private:
		Domain *ptrDomain;
		map< int, double > membership;
		vector< int > pos; 

		FuzzySet *first, *second;

		void insertElement(int, double);
		bool match(Object, Object, int);
		Object createDomainObject(Object, Object, int);
		double calcMembership(Object, Object, string);


	public:
		Composition(FuzzySet*, FuzzySet*, string, Domain*, vector<int>&);
		double getMembership(Object);
		double getMembershipAt(int);
		Domain* getDomain();
};

#endif