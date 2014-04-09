#ifndef DOMAIN_H
#define DOMAIN_H 

#include <iostream>
using namespace std;

#include "func.h"


class Domain {
	private:
		DomainVector domainComponents;

	public:
		virtual long getCardinality()=0;
		virtual int getIndexOfElement(Object)=0;
		virtual Object elementAt(int)=0;
		virtual Object fromStringRepresentation(string)=0;

		void addDomainComponent(Domain*);
		DomainVector getDomainComponents();

		virtual ~Domain(){ }
};


class integer : public Domain {
	private:
		map< int, int > dValue;
		map< int, int > dIndex;
		int lower, upper, step; 

		void insertElement(int, int);

	public:
		integer(int,int,int);
		~integer() { }

		long getCardinality();
		Object elementAt(int);
		int getIndexOfElement(Object);
		Object fromStringRepresentation(string);
};


class real : public Domain {
	private:
		map< int, double > dValue;
		map< double, int > dIndex;
		double lower, upper, step;

		void insertElement(int, double);

	public:
		real(double,double,double);
		~real() { }

		long getCardinality();
		Object elementAt(int);
		int getIndexOfElement(Object);
		Object fromStringRepresentation(string);
};

class enumerated : public Domain {
	private:
		map< int, pair<int,string> > dValue;
		map< string, pair<int,string> > dIndex;
	
		void insertElement(int, pair<int,string>);
	public:
		~enumerated() { }
		enumerated(vector< string>&);

		long getCardinality();
		Object elementAt(int);
		int getIndexOfElement(Object);
		Object fromStringRepresentation(string);
};

class cartesian : public Domain {
	private:
		map< int, string > dKey;
		map< string, int > dIndex;

		void insertDomComp(DomainVector);
		void insertElement(int, string);
		deque<string> generateKeys(vector<int>&);

		bool isIndex(int);
		bool isKey(string);

	public:
		cartesian(DomainVector&);
		~cartesian() { }

		long getCardinality();
		Object elementAt(int);
		int getIndexOfElement(Object);
		Object fromStringRepresentation(string);
		int getIndexFromKey(string);
};

#endif