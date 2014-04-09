#ifndef GA_H
#define GA_H

#include "misc.h"
#include "nn.h"
#include "dataset.h"

class Individual;
typedef shared_ptr<Individual> ptrIndividual;
typedef vector<ptrIndividual> vPopulation;


class GA {
private:
	vPopulation generateRandomPopulation(int,int);
	vector< double > _param;

	double getParameter(int);
	double calcFitness(ptrIndividual&, shared_ptr<NeuralNetwork>&, shared_ptr<DataSet>&);

public:
	GA(vector<double>&);
	~GA() { }

	vector<double> start(shared_ptr<NeuralNetwork>&, shared_ptr<DataSet>&);
};


class Individual {
private:
	double fitness;
	vector< double > chromosome;

public:
	Individual(vector<double>& c) : chromosome(c) { }
	~Individual() { }

	vector<double>& getChromosome();
	void updateComponent(int, double);
	double getComponent(int);
	double getFitness();
	void setFitness(double);
	long getSize();
};



class Selection {
private:
	int k;

public:
	Selection(int _k);
	~Selection() { }
	int select(vPopulation&);
};



class IMutation {
public:
	virtual void mutate(ptrIndividual&)=0;
};


class Mutation1 : public IMutation {
private:
	double sigma, v;
	std::mt19937 generator;

public:
	Mutation1(double sigma, double v, std::mt19937);
	~Mutation1() { }
	void mutate(ptrIndividual&);
};



class Mutation2 : public IMutation {
private:
	double sigma, v;
	std::mt19937 generator;

public:
	Mutation2(double sigma, double v, std::mt19937);
	~Mutation2() { }
	void mutate(ptrIndividual&);
};



class ICrossover {
public:
	virtual void crossover(const ptrIndividual&, const ptrIndividual&, ptrIndividual&)=0;
};



class Arithmetic : public ICrossover {
public:
	Arithmetic() { }
	void crossover(const ptrIndividual&, const ptrIndividual&, ptrIndividual&);
};


class TockaPrekida : public ICrossover {
public:
	void crossover(const ptrIndividual&, const ptrIndividual&, ptrIndividual&);
};


class Uniform : public ICrossover {
public:
	void crossover(const ptrIndividual&, const ptrIndividual&, ptrIndividual&);
};


#endif
