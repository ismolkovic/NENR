#ifndef INFO_H
#define INFO_H 

#include <iostream>
using namespace std;

#include "app.h"

class Info {
	private:

		static Info *instance;
		Info(){ }
		~Info() { }

		map< string, Domain** 	> mapDomain; // zato jer se vrijednost
		map< string, FuzzySet** > mapFuzzySet; // dok imamo redefiniciju fxx 	
		map< string, string > domainString; // pamtimo id  string iz kojeg je nastala domena

		map< string, string > mapOperator;
		vector< FuzzySet** > TreeFuzzySet; // pamtimo cvore operatore da ih kasnije mozemo pobrisati
																			 // buduci da ne mozemo do njih pomocu dfs-a

		map< string, Domain* > mapFuzzyDomain;


	public:
		void registerDomain(string, Domain*, pair<string,string>);
		void registerFuzzySet(string, FuzzySet*);
		void registerOperator(string, string);
		bool isFuzzySetRegistered(string);
		void addTreeFuzzySet(FuzzySet **s) { TreeFuzzySet.push_back(s); }
		void registerFuzzyDomain(string, Domain*);

		// metode za dohvacanje
		Domain* getDomainPointer(string);
		Domain* getDomainFromFuzzyId(string);

		FuzzySet* getFuzzySetPointer(string);
		FuzzySet** getFuzzySetDoublePointer(string);
		
		string getDomainString(string);
		string getDomainStringFromPointer(Domain*);
		string getOperator(string);

		bool cmpDomain(Domain*, Domain*);
		void clean();

		vector< int > findProjectExtensionPositions(string, string);
		vector< int > findChainPositions(string, string);
		
		static Info *Instance() {
			if ( instance == 0) instance = new Info;
			return instance;
		}

		static void Delete() {
			if(instance != NULL) {
				instance->clean();
				delete instance;
			}
			instance = NULL;
		}
};


#endif