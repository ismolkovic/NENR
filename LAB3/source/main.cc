/*
	Copyright (c) 2013. Igor Smolkovič 0036453288

	Fakultet elektrotehnike i računarstva
	Predmet: Neizrazito, evolucijsko i neuro računastvo
				3. dio projektnog zadatka
*/

#include <iostream>
#include <cstdlib>
using namespace std;

#include "stohastic.h"
#include "gradient.h"
#include "system.h"


int main(int argc, char const **argv)
{
	if (argc != 4) {
		cout << "./LAB3 [train-data] [N rules] [type]" << endl;
		return 1;
	}

	Data *d = new Data(argv[ 1 ]);
	int N = atoi(argv[ 2 ]);
	int type = atoi(argv[ 3 ]);

	try {
		if(type == 0) {
			StohasticGD* sGrad = new StohasticGD(N, d);
			sGrad->run(1E-6, 20000, 0.0005);
			sGrad->print();
			delete sGrad; delete d;
		}

		if(type == 1) {
			BatchGD *bGrad = new BatchGD(N, d);
			bGrad->run(1E-6, 20000, 0.0001);
			bGrad->print();
			delete bGrad; delete d;
		}
	}

	catch(string e) {
		cout << e << endl;
	}

	return 0;
}
