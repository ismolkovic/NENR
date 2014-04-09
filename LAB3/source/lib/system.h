#ifndef SYSTEM_HEADER
#define SYSTEM_HEADER

#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

class FuzzySet {
private:
	double a, b, c;
public:
	FuzzySet() { }
	~FuzzySet() { }
	double getMembership(double&, double&, double&, double&);
};


class Antecedent {
public:
	Antecedent() { }
	~Antecedent() { }
	double getW(double&, double&);
};


class Consequent {
public:
	Consequent(){ }
	~Consequent() { }
	double getF(double&, double&, double&, double&, double&);
};


class Output {
public:
	Output() { }
	~Output() { }
	double calcOutput(vector<double>&, vector<double>&);
};

class Data {
private:
	long size;
	vector< double > x;
	vector< double > y;
	vector< double > z;
public:
	Data(const char*); 
	~Data() { }
	double getX(int);
	double getY(int);
	double getZ(int);
	long getSize();
};


class Derivation {
public:
	Derivation() { }
	~Derivation() { }

	double getDeltaP(double&, double&, double&, double&);
	double getDeltaQ(double&, double&, double&, double&);
	double getDeltaR(double&, double&, double&);

	// a, b, c
	double dWi_dmia(double&, double&);
	double dWi_dmib(double&, double&);
	double dOk_dWi(vector<double>&, vector<double>&, int&, double&);

	double dmi_da(double&, double&, double&, double&);
	double dmi_db(double&, double&, double&, double&);
	double dmi_dc(double&, double&, double&, double&);

};

#endif