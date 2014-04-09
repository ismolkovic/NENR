#include "extension.h"

// skup i domena koji se koriste u projekciji
Project::Project(FuzzySet *set, Domain *d, vector<int>& pos) : ptrDomain(d) {
	// na mjestu gdje je pos 0 tamo se trazi maksimum
	Domain *old = set->getDomain();
	
	for(int i = 0; i < old->getCardinality(); i++) {
		Object o = old->elementAt(i);
		double memb = set->getMembership(o);

		Object new_obj = createDomainElement(o, pos);
		projectElement(new_obj, memb);
	}
}

void Project::projectElement(Object o, double m) {
		int index = getDomain()->getIndexOfElement(o);

		if ( getMembershipAt(index) < m) {
			insertMembership(index, m);
		}
}

// ostvorimo novi objekt koji nastaje maksimizacijom
// po necemu 
Object Project::createDomainElement(Object o, vector<int>& pos) {
	vector< Object > vObj = Object2vectorObject(o);
	if (vObj.size() != pos.size()) throw 20;

	vector< Object > temp;
	for(unsigned int j = 0; j < vObj.size(); j++) {
		if(pos.at(j) == 1) temp.push_back(vObj.at(j));
	}
	
	if(temp.size() > 1) return Object(temp);
	else return temp[ 0 ];
}

double Project::getMembership(Object o) {
	int index = getDomain()->getIndexOfElement(o);
	return getMembershipAt(index);
}

Domain* Project::getDomain() {
	if(ptrDomain == NULL) throw 20;
	return ptrDomain;
}


void Project::insertMembership(int index, double value) {
	membership[ index ] = value;
}

double Project::getMembershipAt(int index) {
	if ( membership.find(index) == membership.end()) return 0;
	return membership[ index ];
}


// class CylindricalExtension

CylindricalExtension::CylindricalExtension(FuzzySet *f, Domain *d, vector<int>& pos) :
	ptrDomain(d)
{
	Domain *old = f->getDomain();
	
	for(int i = 0; i < old->getCardinality(); i++) {
		Object o = old->elementAt(i);
		double memb = f->getMembership(o);

		ExtendElement(o, pos, memb, 0);
	}	
}

// rekurzija koja prosiruje svaki element
void CylindricalExtension::ExtendElement(Object o, vector<int>& pos, double value, int i) {
	vector< Object > old;

	if(o.type() == typeid(vector<Object>)) {
		old = Object2vectorObject(o);
	}
	else old.push_back(o);

	// dobili smo pocetni vektor sada samo treba na odgovarajuce pozicije ubaciti elemente
	vector< Domain* > domComps = getDomain()->getDomainComponents();

	if (i == (int)pos.size()) {
		insertElement(Object(old), value);
		return;
	}

	if (pos.at(i) == 0) {
		for(unsigned int j = 0; j < domComps.at(i)->getCardinality(); j++) {
			vector<Object> temp(old);
			Object in = domComps.at(i)->elementAt(j);
			temp.insert(temp.begin() + i, in);

			ExtendElement(Object(temp), pos, value, i + 1);
		}
	}
	else if (pos.at(i) == 1) ExtendElement(Object(old), pos, value, i + 1);
	else throw 20;
}

void CylindricalExtension::insertElement(Object o, double value) {
	int index = getDomain()->getIndexOfElement(o);
	membership[ index ] = value;
}

	
double CylindricalExtension::getMembership(Object o ) {
	int index = getDomain()->getIndexOfElement(o);
	return getMembershipAt(index);
}

double CylindricalExtension::getMembershipAt(int index) {
	if (membership.find(index) == membership.end()) 
		return 0;
	return membership[ index ];
}
		
Domain* CylindricalExtension::getDomain() {
	if (ptrDomain == NULL) throw 20;
	return ptrDomain;
}

// class Composition
// imamo domenu koja nastaje te pozicje koje se ulancavaju u pos
// imamo type jos i relacije iz kojih nastaje kompozicija 
Composition::Composition(FuzzySet *_first, FuzzySet *_second, string type, 
	Domain *d, vector<int>& _pos) : ptrDomain(d), pos(_pos), first(_first), second(_second)
	{
		// tu sad radimo kompoziciju
		Domain *d1 = first->getDomain();
		Domain *d2 = second->getDomain();
		int half = d1->getDomainComponents().size();

		for(int i = 0; i < d1->getCardinality(); i++) {
			Object a = d1->elementAt(i);
			if (first->getMembership(a) == 0) continue;

			for(int j = 0; j < d2->getCardinality(); j++) {
				Object b = d2->elementAt(j);
				if(second->getMembership(b) == 0) continue;

				if(match(a, b, half)) {
					Object out = createDomainObject(a, b, half);
					int index = getDomain()->getIndexOfElement(out);
					double value = calcMembership(a, b, type);

					if(getMembershipAt(index) < value) {
						insertElement(index, value);
					}
				}
			}
		}
}

// metoda koja racuna rezultatnu pripadnost na odredeni nacin
// bilo max-min bilo max-product
double Composition::calcMembership(Object a, Object b, string type) {
	double memb1 = first->getMembership(a);
	double memb2 = second->getMembership(b);
	
	if (type == MAX_MIN) {
		return min(memb1, memb2);
	}
	else if(type == MAX_PRODUCT) {
		return memb1 * memb2;
	}
	else throw 20;
}

// metoda koja provjerava da li imamo element redka prve i stupca druge 
bool Composition::match(Object a, Object b, int half) {
	vector< Object > f = Object2vectorObject(a);
	vector< Object > s = Object2vectorObject(b);

	int j = half;
	for(int i = 0; i < half; i++) {
		if (pos.at(i) == 0) continue;
		if (f.at(i) == s.at(j-half)) { j++;}
		else return false;
	}

	return true;
}

// metoda koja kreira odgovorajuci element rezultatne domene
Object Composition::createDomainObject(Object a, Object b, int half) {
	vector<Object> ret;

	vector< Object > f = Object2vectorObject(a);
	vector< Object > s = Object2vectorObject(b);

	for(int i = 0; i < half; i++) {
		if(pos.at(i) == 0) ret.push_back(f.at(i));
	}

	for(int i = half; i < (int)pos.size(); i++) {
		if(pos.at(i) == 0) ret.push_back(s.at(i-half));
	}


	if (ret.size() == 1) return Object(ret.at(0));
	return Object(ret);
}

Domain* Composition::getDomain() {
	if (ptrDomain != NULL ) return ptrDomain;
	else throw 20;
}

double Composition::getMembership(Object o) {
	int index = getDomain()->getIndexOfElement(o);
	return getMembershipAt(index);
}
		
double Composition::getMembershipAt(int index) {
	if (membership.find(index) == membership.end()) 
		return 0;
	return membership[ index ];
}

void Composition::insertElement(int index, double value) {
	membership[ index ] = value;
}