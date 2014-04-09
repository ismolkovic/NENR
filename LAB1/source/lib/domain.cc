#include "domain.h"


void Domain::addDomainComponent(Domain *d) {
	domainComponents.push_back( d );
}
DomainVector Domain::getDomainComponents() { 
	return domainComponents; 
}

// integer
long integer::getCardinality() { return dValue.size(); }
int integer::getIndexOfElement(Object value) {
	int val = Object2int(value);
	if(dIndex.find(val) == dIndex.end()) throw 10;
	return dIndex[ val ];
}

integer::integer(int l, int u, int s) : lower(l), upper(u), step(s) {
	for(int value = lower, index = 0; value <= upper; value += step, index++) {
		insertElement(index, value);
	}

	addDomainComponent(this);
}

void integer::insertElement(int index, int value) {
	dIndex[ value ] = index;
	dValue[ index ] = value;
}

Object integer::elementAt(int index) {
	if (dValue.find(index) == dValue.end()) throw 11;
	else return Object(dValue[ index ]);
}

Object integer::fromStringRepresentation(string input) {
	Object ret = string2int(input);
	return ret;
}

// real
long real::getCardinality() { return dValue.size(); }
int real::getIndexOfElement(Object value) {
	double val = Object2double(value);
	if(dIndex.find(val) == dIndex.end()) throw 10;
	return dIndex[ val ];
}

real::real(double l, double u, double s) : lower(l), upper(u), step(s) {
	int index = 0;
	for(double value = lower; value <= upper; value += step) {
		value = round(value * 1000000) / 1000000; // zaokruzimo na 6 decimala
		insertElement(index++, value);
	}

	addDomainComponent(this);
}

void real::insertElement(int index, double value) {
	dIndex[ value ] = index;
	dValue[ index ] = value;
}

Object real::elementAt(int index) {
	if (dValue.find(index) != dValue.end()) {
		return Object(dValue[ index ]) ;
	}
	throw 11;
}

Object real::fromStringRepresentation(string input) {
	Object ret = string2double(input);
	return ret;
}

//enumerated
long enumerated::getCardinality() { return dValue.size(); }
int enumerated::getIndexOfElement(Object value) {
	string val = ObjectPair2string(value);
	if(dIndex.find(val) == dIndex.end()) throw 10;
	return dIndex[ val ].first;
}

enumerated::enumerated(vector< string> &vals) {
	int index = 0;
	for(vector<string>::iterator i  = vals.begin(); 
		i != vals.end(); i++) {
			insertElement(index, make_pair(index,*i));
			index++;
		}

	addDomainComponent(this);
}

void enumerated::insertElement(int index, pair<int,string> value) {
	dIndex[ value.second ] = value;
	dValue[ index ] = value;
}

Object enumerated::elementAt(int index) {
	if (dValue.find(index) != dValue.end()) {
		return Object(dValue[ index ]) ;
	}
	throw 11;
}

Object enumerated::fromStringRepresentation(string input) {
	if (dIndex.find(input) != dIndex.end()) return Object(dIndex[ input ]);
	throw 20;
	
}

// cartesian
cartesian::cartesian(DomainVector& vect) {
	vector< int > cardinalitets;
	for(unsigned int i = 0; i < vect.size(); i++) {
		insertDomComp(vect[ i ]->getDomainComponents());
		cardinalitets.push_back(vect[i]->getCardinality());
	}

	deque< string > keys = generateKeys(cardinalitets);
	int index = 0;
	
	while (!keys.empty()) {
		insertElement(index++, keys.front()); 
		keys.pop_front();
	}
}

deque<string> cartesian::generateKeys(vector<int>& cards) {
	deque<string> ret;
	for(int i = 0; i < cards[ 0 ]; i++) {
		ret.push_back(int2string(i));
	}
	
	for(unsigned int j = 1; j < cards.size(); j++) {
		
		unsigned int n = ret.size();
		for(unsigned int k = 0; k < n; k++) {
			
			string current = ret.front(); ret.pop_front();
			for(unsigned int l = 0; l < (unsigned)cards[ j ]; l++) {
				string value =  current + "," + int2string(l);
				ret.push_back(value);
			}
		}
	}

	return ret;
}

void cartesian::insertDomComp(DomainVector comp) {
	for(vector< Domain* >::iterator i = comp.begin();
		i != comp.end(); i++)
	{
		this->addDomainComponent(*i);
	}
}

void cartesian::insertElement(int index, string value) {
	dIndex[ value ] = index;
	dKey[ index ] = value;
}

long cartesian::getCardinality() { return dKey.size(); }
Object cartesian::elementAt(int index) {
	vectorObject  ret;
	
	if (isIndex(index) == false) throw 20;
	vector< string > keys = splitString(removeSpaces(dKey[ index ]), ",");

	for(unsigned int i = 0; i < keys.size(); i++) {
		ret.push_back(getDomainComponents()[i]->elementAt(string2int(keys[ i ])));
	}

	return Object(ret);
}


int cartesian::getIndexOfElement(Object obj) {
	vectorObject temp = Object2vectorObject(obj);
	string key = "";
	for(unsigned int i = 0; i < temp.size(); i++) {
		int index = getDomainComponents()[i]->getIndexOfElement(temp[i]);
		key = key + int2string(index);
		if ( i < temp.size() - 1) key = key + ",";
	}

	if (isKey(key) == false) throw 16; 

	return dIndex[ key ];
}


Object cartesian::fromStringRepresentation(string input) {
	vectorObject ret;

	vector<string> values = splitString(input, ",");
	for(unsigned int i = 0; i < values.size(); i++) {
		ret.push_back(getDomainComponents()[i]->fromStringRepresentation(values[i]));
	} 

	return Object(ret);
}

bool cartesian::isIndex(int index) {
	return ! (dKey.find(index) == dKey.end());
}

bool cartesian::isKey(string key) {
	return ! (dIndex.find(key) == dIndex.end());
}

int cartesian::getIndexFromKey(string key) {
	if(isKey(key) == false) throw 16;
	return dIndex[ key ];
}