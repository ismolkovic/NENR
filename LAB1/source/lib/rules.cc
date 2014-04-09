#include "rules.h"


long Rules::getNumberOfRules() {
	return Nr;
}

map< int, FuzzySet* >& Rules::getAntecedent(int r) {
	if(mapAntecedent.find(r) != mapAntecedent.end()) 
		return mapAntecedent[ r ];
	
	else throw 40;
}


FuzzySet* Rules::getConsequent(int r) {
	if(mapConsequent.find(r) != mapConsequent.end())
		return mapConsequent[ r ];

	throw 40;
}

void Rules::insertRule(map< int, FuzzySet* >& A, FuzzySet *C) {
	mapAntecedent[ Nr ] = A;
	mapConsequent[ Nr++ ] = C;
}

Rules::~Rules() {
	for(map<int,FuzzySet*>::iterator i = mapConsequent.begin();
		i != mapConsequent.end(); i++) 
	{
		delete i->second;
	}

	map< int, map< int, FuzzySet* > >::iterator j = mapAntecedent.begin();
	for( ; j != mapAntecedent.end(); j++) {
		map< int, FuzzySet* >& temp = j->second;

		for(map< int, FuzzySet*>::iterator k = temp.begin(); k != temp.end(); k++)
			delete k->second;
	}
}


/*
		OVDJE DEFINIRAMO PRAVILA ZA KORMILO
*/


void Rules::initKormilo() {
	Domain *d1 = new integer(0, 1300, 1);
	Domain *d2 = new integer(-90, 90, 1);
	Domain *d3 = new integer(0, 1, 1);

	
	// R1 mora ici ravno
	FuzzySet *malaL = new L(d1, Object(int(38)), Object(int(80)));
	FuzzySet *malaD = new L(d1, Object(int(38)), Object(int(80)));
	FuzzySet *malaLK = new L(d1, Object(int(38)), Object(int(80)));
	FuzzySet *malaDK = new L(d1, Object(int(38)), Object(int(80)));

	FuzzySet *Y1 = new Lambda(d2, Object(int(-5)), Object(int(0)), Object(int(5)));

	map< int, FuzzySet* > R1; 
	R1[ 0 ] = malaL; R1[ 1 ] = malaD; R1[ 2 ] = malaLK; R1[ 3 ] = malaDK;

	insertRule(R1, Y1);


	// R2 definira srednji LK mora skrenuti lijevo  do max 30

	FuzzySet *srednjiLK = new Lambda(d1, Object(int(80)), Object(int(400)), Object(500));
	FuzzySet *Y2 = new Pi(d2, Object(int(19)), Object(20), Object(39), Object(int(40)));

	map< int, FuzzySet* > R2; 
	R2[ 2 ] = srednjiLK;

	insertRule(R2, Y2);



	// R3 definira srednji DK skrece desno do max -30

	FuzzySet *srednjiDK = new Lambda(d1, Object(int(80)), Object(int(400)), Object(500));
	FuzzySet *Y3 = new Pi(d2, Object(int(-40)), Object(-39), Object(-20), Object(int(-19)));


	map< int, FuzzySet* > R3; 
	R3[ 3 ] = srednjiDK;

	insertRule(R3, Y3);



	// R4 definira veliki LK srene do max  90
	FuzzySet *velikiLK = new Gamma(d1, Object(int(450)), Object(500));
	FuzzySet *Y4 = new Gamma(d2, Object(int(70)), Object(75));

	map< int, FuzzySet* > R4; 
	R4[ 2 ] = velikiLK;

	insertRule(R4, Y4);

	// R5 definira veliki DK skrene do max -90
	FuzzySet *velikiDK = new Gamma(d1, Object(int(450)), Object(500));
	FuzzySet *Y5 = new L(d2, Object(int(-75)), Object(-70));


	map< int, FuzzySet* > R5; 
	R5[ 3 ] = velikiDK;

	insertRule(R5, Y5);

	
	//R6 
	// treba jos pravilo ako je jako blizu lijevo ili jako blizu desno

	FuzzySet *jakoMalaL = new L(d1, Object(int(30)), Object(int(38)));
	FuzzySet *Y6 = new Pi(d2, Object(int(-56)), Object(-55), Object(-36), Object(int(-35)));

	map< int, FuzzySet*> R6; R6[ 0 ] = jakoMalaL;
	insertRule(R6, Y6);

	// R7
	FuzzySet *jakoMalaD = new L(d1, Object(int(30)), Object(int(38)));
	FuzzySet *Y7 = new Pi(d2, Object(int(35)), Object(36), Object(55), Object(int(56)));

	map< int, FuzzySet*> R7; R7[ 1 ] = jakoMalaD;
	insertRule(R7, Y7);


	//R8
	// treba jos pravilo ako je jako blizu lijevo ili jako blizu desno

	FuzzySet *jakoMalaLK = new L(d1, Object(int(30)), Object(int(38)));
	FuzzySet *Y8 = new Pi(d2, Object(int(-56)), Object(-55), Object(-36), Object(int(-35)));

	map< int, FuzzySet*> R8; R8[ 2 ] = jakoMalaLK;
	insertRule(R8, Y8);

	// R9
	FuzzySet *jakoMalaDK = new L(d1, Object(int(30)), Object(int(38)));
	FuzzySet *Y9 = new Pi(d2, Object(int(35)), Object(36), Object(55), Object(int(56)));

	map< int, FuzzySet*> R9; R9[ 3 ] = jakoMalaDK;
	insertRule(R9, Y9);

	
	// R10 ako je krivi smjer onda okreni brodić za 90 stupnjeva
	// ako je nula onda bude iznos jako veliki te treba okrenuti brod za 90 stupnjeva 
	FuzzySet *Smjer = new L(d3, Object(int(0)), Object(int(1)));
	FuzzySet *Y10 = new L(d2, Object(int(-75)), Object(int(-70)));

	map<int, FuzzySet*> R10;
	R10[ 5 ] = Smjer;
	insertRule(R10, Y10);

}

/*	
		OVDJE DEFINIRAMO PRAVILA ZA AKCELERACIJU
*/


void Rules::initAkceleracija() {
	// ako je pri rubu malo povecati brzinu

	Domain *d1 = new integer(0, 1300, 1);
	Domain *d2 = new integer(-150, 150, 1);
	Domain *d3 = new integer(0, 150, 1);

	// R1 mora ici ravno
	FuzzySet *malaL = new L(d1, Object(int(38)), Object(int(80)));
	FuzzySet *malaD = new L(d1, Object(int(38)), Object(int(80)));
	FuzzySet *malaLK = new L(d1, Object(int(38)), Object(int(80)));
	FuzzySet *malaDK = new L(d1, Object(int(38)), Object(int(80)));

	FuzzySet *Y1 = new Lambda(d2, Object(int(0)), Object(int(5)), Object(int(6)));

	map< int, FuzzySet* > R1; 
	R1[ 0 ] = malaL; R1[ 1 ] = malaD; R1[ 2 ] = malaLK; R1[ 3 ] = malaDK;

	insertRule(R1, Y1);

	// R2 nosi ga k rubu

	FuzzySet *jakoMalaL = new L(d1, Object(int(20)), Object(int(38)));
	FuzzySet *Y2 = new Lambda(d2, Object(int(7)), Object(int(8)), Object(int(9)));

	map< int, FuzzySet*> R2; R2[ 0 ] = jakoMalaL;
	insertRule(R2, Y2);


	// R3 nosi ga k rubu 
	FuzzySet *jakoMalaD = new L(d1, Object(int(20)), Object(int(38)));
	FuzzySet *Y3 = new Lambda(d2, Object(int(7)), Object(int(8)), Object(int(9)));

	map< int, FuzzySet*> R3; R3[ 1 ] = jakoMalaD;
	insertRule(R3, Y3);


	// R4 ako je brzina oko 70 onda ju smanji akcelaracijom 5

	FuzzySet *brzina60 = new Lambda(d3, Object(int(55)), Object(int(60)),  Object(int(65)) );
	FuzzySet *Y4 = new Lambda(d2, Object(int(-7)),  Object(int(-6)), Object(int(-5)) );

	map< int, FuzzySet* > R4; R4[ 4 ] = brzina60; 
	insertRule(R4, Y4);

}