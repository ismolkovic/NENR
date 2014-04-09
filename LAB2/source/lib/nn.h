#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "misc.h"
#include "dataset.h"


/*
	Klasa koja implementira funkcionalnosti neuronske mreže 
		- primjena modela
		- računanje pogreške
		- računanje izlaza za TIP1 i TIP2 neurona
		- dohvata informacija o parametrima i slojevima
*/
class NeuralNetwork {
private:
	vector<int> _info;
	long _neuronsNumber, _paramNumber;
	vector< double > _output;

	double getType1Output(vector<double>&, vector<double>&, int, int, int);
	double getType2Output(vector<double>&, vector<double>&, int, int, int);
	long getSizeOfLayer(int);
	long getNumberOfLayers();
	long getNumberOfNeurons();
	bool checkOutput(vector<double>&, vector<double>&);

public:
	NeuralNetwork(vector<int>&);
	~NeuralNetwork() { }

	long getNumberOfParameters();
	vector< double > calcOutput(vector<double>&, SAMPLE&);
	double calcError(vector<double>&, shared_ptr<DataSet>&);	
	vector< double > decodeOutput(vector<double>&);
	void printSolution(vector<double>&, shared_ptr<DataSet>&);
};

#endif