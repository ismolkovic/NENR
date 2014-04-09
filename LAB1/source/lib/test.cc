#include "test.h"

namespace Test {

	bool testCartesianSelf(Domain *d) {
		Domain *first = d->getDomainComponents()[ 0 ];
		Domain *second = d->getDomainComponents()[ 1 ];

		return first == second; 
	}

	bool testReflexive(FuzzySet *set) {

		// prvo mora biti napravljena na domeni koja je na kartezijevu produktu dviju istih domena
		if (set->getDomain()->getDomainComponents().size() != 2) throw 20;
		if (Test::testCartesianSelf(set->getDomain()) == false) throw 20;

		// tu sad ide dalje provjera;
		Domain *d = set->getDomain()->getDomainComponents()[ 0 ];
		for(int i = 0; i < d->getCardinality(); i++) {
			Object a = d->elementAt(i);
			Object o = pairObject(a,a);

			if (set->getMembership(Object(o)) != (double)1) return false;
		}
		return true;
	}

	bool testSymmetric(FuzzySet *set) {
		
		// prvo mora biti napravljena na domeni koja je na kartezijevu produktu dviju istih domena
		if (set->getDomain()->getDomainComponents().size() != 2) throw 20;
		if (Test::testCartesianSelf(set->getDomain()) == false) throw 20;

		Domain *d = set->getDomain()->getDomainComponents()[ 0 ];
		for(int i = 0; i < d->getCardinality() - 1; i++) {
			for(int j = i+1; j < d->getCardinality(); j++) {
				Object a = d->elementAt(i);
				Object b = d->elementAt(j);

				double memb1 = set->getMembership(pairObject(a,b));
				double memb2 = set->getMembership(pairObject(b,a));

				if (memb1 != memb2) return false;
				if (memb1 == 0 && memb2 == 0) return false;
			}
		}
		return true;
	}

	// type je max-min ili max-product
	bool testTransitive(FuzzySet *set, string type) {
		// prvo mora biti napravljena na domeni koja je na kartezijevu produktu dviju istih domena
		if (set->getDomain()->getDomainComponents().size() != 2) throw 20;
		if (Test::testCartesianSelf(set->getDomain()) == false) throw 20;

		if( type == MAX_MIN ) return testMaxMin(set);
		else if (type == MAX_PRODUCT ) return testMaxProduct(set);
		else throw 20;
	}

	bool testMaxMin(FuzzySet *set) {
		Domain *d = set->getDomain()->getDomainComponents()[ 0 ];
		int N = d->getCardinality();
		for(int x = 0; x < N; x++) {
			for(int z = 0; z < N; z++) {
				for(int y = 0; y < N; y++) {
					Object X = d->elementAt(x);
					Object Y = d->elementAt(y);
					Object Z = d->elementAt(z);

					double memb1 = set->getMembership(pairObject(X,Z));
					double memb2 = set->getMembership(pairObject(X,Y));
					double memb3 = set->getMembership(pairObject(Y,Z));

					if ( memb1 < min(memb2, memb3)) return false;
				}
			}
		}

		return true;
	}

	bool testMaxProduct(FuzzySet *set) {
		Domain *d = set->getDomain()->getDomainComponents()[ 0 ];
		int N = d->getCardinality();
		for(int x = 0; x < N; x++) {
			for(int z = 0; z < N; z++) {
				for(int y = 0; y < N; y++) {
					Object X = d->elementAt(x);
					Object Y = d->elementAt(y);
					Object Z = d->elementAt(z);

					double memb1 = set->getMembership(pairObject(X,Z));
					double memb2 = set->getMembership(pairObject(X,Y));
					double memb3 = set->getMembership(pairObject(Y,Z));

					if ( memb1 < memb2 * memb3) return false;
				}
			}
		}

		return true;
	}

	// type je max-min ili max-product
	bool testFuzzyEquivalence(FuzzySet *set, string type) {
		if(testReflexive(set) == false) return false;
		if(testSymmetric(set) == false) return false;
		if(testTransitive(set, type) == false) return false;

		return true;
	}

}