
/*	
	Fakultet elektrotehnike i računarstava

		Neizrazito, evolucijsko i neuro računarstvo

		LAB1 

		Igor Smolkovič 0036453288

	Program koji se koristi u 1/3 i 2/3

*/

#include <iostream>
using namespace std;

#include "app.h"

Parser *Parser::instance = 0;
Info *Info::instance = 0;

int main(int argc, char **argv) {
	try {
		if(argc == 2) {
			Parser::Instance()->parse(argv[ 1 ]);
			Info::Delete();
			Parser::Delete();
		}

		else cout << "Usage: ./LAB1 <file>" << endl;
	}

	catch (int e) {
		if (e == 22) cout << "Bad file" << endl;
	}
	return 0;
}
