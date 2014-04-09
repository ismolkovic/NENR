/*
	Copyright (c) 2013. Igor Smolkovič 0036453288

	Fakultet elektrotehnike i računarstva
	Predmet: Neizrazito, evolucijsko i neuro računastvo
				3. dio projektnog zadatka - Generiranje primjera za ucenje
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <fstream>
using namespace std;

//f1 (x, y) = ((x − 1)2 + (y + 2)2 − 5xy + 3) ∗ cos2 ( x )
double f1(double x, double y) {
	double val = ((x-1) * (x-1) + (y+2) * (y+2) - 5*x*y + 3);
	val = val * cos(x/5) * cos(x/5);
	return val; 
}

int main(int argc, char **argv) {
	if(argc != 2) {
		cout << "./genTrainData [out file]" << endl;
		return 0;
	}

	FILE *f = fopen(argv[ 1 ], "w");

	for(int i = -4; i <= 4; i++) {
		for(int j = -4; j <= 4; j++) {
			fprintf(f, "%d %d %.10lf\n", i, j, f1(i,j));
		}
	}

	fclose(f);
	return 0;
}