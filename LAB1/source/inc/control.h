#ifndef CONTROL_H
#define CONTROL_H 

#include <iostream>
using namespace std;

#include "domain.h"
#include "fuzzyset.h"
#include "rules.h"

class Decoder;
class FuzzyControl {
	public:
		virtual ~FuzzyControl() { }
		virtual double getOutput(vector<Object>&)=0;
};


class MinimumEngine : public FuzzyControl {
	private:
		Rules *rules;
		Decoder *decoder;

	public:
		MinimumEngine(Rules *r, Decoder *d);
		double getOutput(vector<Object>&);
		Rules* getRules();
		Decoder* getDecoder();
		vector< double > convertToMembership(vector<Object>&, int);
};


class ProductEngine : public FuzzyControl {
	private:
		Rules *rules;
		Decoder *decoder;

	public:
		ProductEngine(Rules *r, Decoder *d);
		~ProductEngine(){ }
		double getOutput(vector<Object>&);
		vector< double > convertMembership(vector<Object>&, int);
		vector< double > convertToMembership(vector<Object>&, int);
};



// dekodiranje neizrazitosti
class Decoder {
	public:
		virtual double calculate(FuzzySet*)=0;
		virtual ~Decoder() { }
};


class CenterOfArea : public Decoder {
	public:
		double calculate(FuzzySet*);
		~CenterOfArea() { }
};


#endif