/*
	Copyright (c) 2013. Igor Smolkovič

	Fakultet elektrotehnike i računarstva
	Predmet: Neizrazito, evolucijsko i neuro računastvo
				2. dio projektnog zadatka

*/

#include "misc.h"
#include "dataset.h"
#include "nn.h"
#include "ga.h"

int main(int argc, char const *argv[])
{
	try{
		shared_ptr< DataSet > in = make_shared<DataSet>(argv[ 1 ]);
		vector<int> info = { 2, 8, 4, 3 };
		shared_ptr< NeuralNetwork > nn = make_shared<NeuralNetwork>(info);

		// 0:sigma1, 1:sigma2, 2:p1, 3:p2, 4:v, 5:iteracija, 6:eps, 7:NC 8:min, 9:max, 10:k
		vector< double > gaParams = { 0.5, 0.5, 0.0225, 0.0025, 0.97, 250000, 1E-7, 15, -1, 1, 3 };
		
		shared_ptr< GA > ga = make_shared<GA>(gaParams);
		vector<double> solution = ga->start(nn, in);

		nn->printSolution(solution, in);
	}

	catch (string e) {
		cout << e << endl;
	}

	return 0;
}
