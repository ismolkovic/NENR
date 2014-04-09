#include "info.h"

// Info class
// 
void Info::registerFuzzySet(string key, FuzzySet* f) { 
	if(mapFuzzySet.find(key) == mapFuzzySet.end()) {
		mapFuzzySet[ key ] = new FuzzySet*;
	}

	*mapFuzzySet[ key ] = f;
	registerFuzzyDomain(key, f->getDomain());
}

	void Info::registerFuzzyDomain(string key, Domain *d) {
		if(d == NULL || key.size() == 0) throw 15;
		mapFuzzyDomain[ key ] = d;
	}

Domain* Info::getDomainFromFuzzyId(string key) {
	if(mapFuzzyDomain.find(key) == mapFuzzyDomain.end()) throw 14;
	return mapFuzzyDomain[ key ];
}


FuzzySet* Info::getFuzzySetPointer(string key){
	if( mapFuzzySet.find(key) == mapFuzzySet.end()) throw 14;
	if(*mapFuzzySet[ key ] != NULL) return *mapFuzzySet[ key ];
	throw 14;
}

FuzzySet** Info::getFuzzySetDoublePointer(string key){
	if( mapFuzzySet.find(key) == mapFuzzySet.end()) throw 14;
	if( *mapFuzzySet[ key ] != NULL) return mapFuzzySet[ key ];
	throw 14;
}

bool Info::isFuzzySetRegistered(string key) {
	return ! (mapFuzzySet.find(key) == mapFuzzySet.end());
}

void Info::registerDomain(string key, Domain *d, pair<string,string> inStr) {
	if(mapDomain.find(key) == mapDomain.end()) {
		mapDomain[ key ] = new Domain*;
		*mapDomain[ key ] = NULL;
	}

	if(*mapDomain[ key ] != NULL ) delete *mapDomain[ key ];
	
	if(inStr.first == "cartesian")
		domainString[ Domain2string(d) ] = removeSpaces(inStr.second);
	else domainString[ Domain2string(d) ] = removeSpaces(key);

	*mapDomain[ key ] = d; 
}

Domain* Info::getDomainPointer(string key) { 
	if ( mapDomain.find(key) == mapDomain.end()) throw 14;
	if (*mapDomain[ key ] != NULL) return *mapDomain[ key ];
	throw 14; 
}


string Info::getDomainString(string key) {
	if(domainString.find(key) == domainString.end()) throw 14;
	return domainString[ key ];
}

string Info::getDomainStringFromPointer(Domain *d) {
	string name = Domain2string(d);
	return getDomainString(name);
}


bool Info::cmpDomain(Domain *first, Domain *second) {
	if(first->getCardinality() != second->getCardinality())
		return false;

	for(int i = 0; i < first->getCardinality(); i++) {
		if(!(first->elementAt(i) == second->elementAt(i))) return false;
	}

	return true; 
}

void Info::registerOperator(string key, string value) {
	mapOperator[ key ] = value;
}

string Info::getOperator(string key) {
	if( mapOperator.find(key) != mapOperator.end()) return mapOperator[ key ];
	if( key == "*" ) return DEFAULT_T;
	if( key == "+" ) return DEFAULT_S;
	if( key == "!" ) return DEFAULT_NOT;
	if( key == "->") return MAMDANI;	

	throw 12;
}

void Info::clean() {
	for(map<string,FuzzySet**>::iterator j = mapFuzzySet.begin();
				j != mapFuzzySet.end(); j++) 
	{
		//cout << "Brisem " << j->first << endl;
		if( *(j->second) != NULL)
			delete *(j->second);

		delete j->second;
	}

	for(map<string,Domain**>::iterator i = mapDomain.begin();
		i != mapDomain.end(); i++) 
	{
		//cout << "Brisem " << i->first << endl;
		if (*(i->second) != NULL)
			delete *(i->second);

		delete i->second;
	}

	// brise ono sto je alocirano za operatorska stabla jer ne mozemo
	// doci do tih elemenata setnjom po stablu
	for(unsigned int k = 0; k < TreeFuzzySet.size(); k++) {
		delete *TreeFuzzySet[k];
		delete TreeFuzzySet[k];
	}
}

// provjeravamo da li je domena d2 podskup od d1 i ako je onda 
// odredimo pozicije na kojim se mora maksimizirati kod projekcije
vector< int > Info::findProjectExtensionPositions(string d1, string d2) {
	vector<string> first = splitString(d1, ",");
	vector<string> second = splitString(d2, ",");
	vector< int > right(first.size(), 0);
	vector< int > left(first.size(), 0);
	
	// pogledamo s jedne i s druge strane kako se moze poravnati ak nije isto 
	// bacimo iznimku
	int j = second.size() - 1;
	for(int i = (int)first.size() - 1; i >= 0 && j>=0; i--) {
		if (first.at(i) == second.at(j)) {
			right.at(i) = 1; j--;
		}
	}

	j = 0;
	for(int i = 0; i < (int)first.size() && j < (int)second.size(); i++) {
		if (first.at(i) == second.at(j)) {
			left.at(i) = 1; j++;
		}
	}

	
	bool subSet = false;
	for(unsigned int i = 0; i < left.size(); i++) {
		if ( left[ i ] != right[ i ]) throw 20;
		if ( left[ i ] == 1) subSet = true;
	}
	
	// ako nije podskup dojaviti to
	if (subSet == false) throw 20;
	
	return right;
}

vector< int > Info::findChainPositions(string d1, string d2) {
	vector< string > first = splitString(d1, ",");
	vector< string > second = splitString(d2, ",");

	// binarne relacije moraju biti oblika X x Y sa Y x Z daje X x Z
	if(first.size() == 2 && second.size() == 2) { 
		if(first.at(1) != second.at(0)) throw 20;
		else {
			vector<int> ret = { 0, 1, 1, 0 };
			return ret;
		}
	}

	// prva domena kartezijev produkt
	// oblika početak x zajedničko a druga domena oblika zajedničko x kraj
	int N; 
	for(N = first.size(); N > 0; N--) {
		int k = 0; bool OK = true; 
		for(int j = first.size() - N; j < (int)first.size(); j++) {
			if (k < (int)second.size()) {
				if(first.at(j) == second.at(k)) k++;
				else { OK = false; break; }
			}
		}
		if ( OK ) break;
	}

	if( N == 0 ) throw 20;

	// inicijaliziramo vektor
	vector< int > ret;
	for(int i = first.size(); i > 0; i--) ret.push_back( i <= N ? 1 : 0 );
	for(int i = 1; i <= (int)second.size(); i++) ret.push_back( i <= N ? 1 : 0 );

	return ret;
}