#ifndef PARSE_H
#define PARSE_H

#include <iostream>
using namespace std;

#include "func.h"
#include "domain.h"
#include "fuzzyset.h"
#include "operator.h"
#include "factory.h"
#include "info.h"
#include "test.h"
#include "extension.h"


class Parser {
	private:
		static Parser *instance;
		Parser(){ }
		~Parser() { }

		// metode za parsiranje
		void parseDomain(string);
		void parseFuzzySet(string);
		void parseWrite(string);
		void parseExpression(string);
		void parseOperator(string);
		void parseSymmetric(string);
		void parseReflexive(string);
		void parseTransitive(string);
		void parseFuzzyEquivalence(string);
		void parseProject(string);
		void parseCylindrical_extension(string);
		void parseComposition(string);


		node* createTree(string input); // napravi sintaksno stablo
		FuzzySet** createOperatorTree(node*);
		void printTree(node*,int);

	public:
		static Parser *Instance() {
			if ( instance == 0)  instance = new Parser;

			return instance;
		}

		static void Delete() {
			if(instance != NULL) {
				delete instance;
			}
			instance = NULL;
		}

		void parse(const char*);
};


// syntax tree 
class node {
	public:
		virtual string getValue()=0;
		virtual ~node(){ }
};

class leaf : public node {
	private:
		string fs; // fuzzyset identifikator fxx
	public:
		leaf(string p) : fs(p) { }
		string getValue() { return fs; }
		~leaf() { }
};

class operation : public node {
	private:
		node *left, *right;
		string op; // operator

	public:
		operation(node *l, node *r, string _op) : left(l), right(r), op(_op) { }
		node *getLeft() { return left; }
		node *getRight() { return right; }
		string getValue() { return op; }
		~operation(){ }
};





#endif
