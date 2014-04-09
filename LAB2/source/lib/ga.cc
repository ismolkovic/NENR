#include "ga.h"

/*
	Kontruktor
	param: polje parametara
*/
GA::GA(vector<double> &p) : _param(p) { }


/*
	Metoda koja vraca parametar na određenom indeksu
	param: index trazenog parametra
*/
double GA::getParameter(int i) {
	if (i >= 0 && i < (signed)_param.size()) 
		return _param.at(i);
	throw EXCEPTION_VECTOR_RANGE;
}


/*
	Metoda koja pokrece genetski algoritam
	param: referenca na NeuralNetwork i referenca na DataSet
	return: vraca vektor parametara nn koji daje najbolje rjesenje
*/
vector<double> GA::start(shared_ptr<NeuralNetwork> &nn, shared_ptr<DataSet> &in) {
	srand(unsigned(time(NULL)));

	int numberComp = nn->getNumberOfParameters(); // broj komponenti
	int numberInd  = getParameter(7); // broj jedinki
	int iter = getParameter(5); double eps = getParameter(6);
	vPopulation population = generateRandomPopulation(numberInd, numberComp);

	// postavimo pocetni fitness
	for(int i = 0; i < numberInd; i++) {
		double fit = calcFitness(population.at(i), nn, in);
		population.at(i)->setFitness(fit);
	}

	std::random_device rd;
	shared_ptr<IMutation> mut1 = make_shared<Mutation1>(getParameter(0), getParameter(2), std::mt19937(rd()));
	shared_ptr<IMutation> mut2 = make_shared<Mutation2>(getParameter(1), getParameter(3), std::mt19937(rd()));
	double v = getParameter(4);

	shared_ptr<Selection> sel = make_shared<Selection>(getParameter(10));
	shared_ptr<ICrossover> cros1 = make_shared<TockaPrekida>();
	shared_ptr<ICrossover> cros2 = make_shared<Arithmetic>();
	shared_ptr<ICrossover> cros3 = make_shared<Uniform>();
	
	int last, best, k;
	for(k = 0; k < iter; k++) {
		//Pronađi najlošiju i najbolju jedinku
		last = best = 0;
		for(int i = 1; i < numberInd; i++) {
			if(population.at(i)->getFitness() > population.at(last)->getFitness())
				last = i;
			if(population.at(i)->getFitness() < population.at(best)->getFitness())
				best = i;
		}

		if(k % (1000 * numberInd) == 0)
			cout << "Greska: " << population.at(best)->getFitness() << endl;
		if(population.at(best)->getFitness() < eps) break;

		int fP = sel->select(population);
		int sP = sel->select(population);
		int R = rand() % 3;

		if ( R == 0)
			cros1->crossover(population.at(fP), population.at(sP), population.at(last));
		else if (R == 1)
			cros2->crossover(population.at(fP), population.at(sP), population.at(last));
		else 
			cros3->crossover(population.at(fP), population.at(sP), population.at(last));

		
		if((double)rand()/RAND_MAX < v) 
			mut1->mutate(population.at(last));
		else 
			mut2->mutate(population.at(last)); 

		double childFit = calcFitness(population.at(last), nn, in);
		population.at(last)->setFitness(childFit);
	}

	cout << endl << "Broj iteracija: " << k << endl;
	return population.at(best)->getChromosome();
}

/*
	Metoda koja slucajno generira populaciju
	param: broj jedinki, broj komponenti jedinke
*/
vPopulation GA::generateRandomPopulation(int NI, int NC) {
	vPopulation ret;
	double min = getParameter(8);
	double max = getParameter(9);

	for(int i = 0; i < NI; i++) {
		vector< double > currentI;
		for(int j = 0; j < NC; j++) {
			double R = (double)rand() / RAND_MAX;
			double val = min + R *(max - min);
			currentI.push_back(val);

		}

		ptrIndividual ind = make_shared<Individual>(currentI); 
		ret.push_back(ind);
	}

	return ret;
}


/*
	Metode koje implementiraju funcionalnost kromosoma
	- stvaranje, modifikaciju, dodjelu dobrote
*/
vector<double>& Individual::getChromosome() { return chromosome; }
	
void Individual::updateComponent(int i, double v) {
	if (i >= 0 && i < (signed)chromosome.size()) 
		chromosome[ i ] = v;
	else throw EXCEPTION_CHROMOSOME_LENGTH;
}

double Individual::getFitness() { return fitness; }

void Individual::setFitness(double f) { fitness = f; }

long Individual::getSize() { return chromosome.size(); }

double Individual::getComponent(int i) { 
	if (i >= 0 && i < (signed)chromosome.size()) 
		return chromosome.at(i);
	throw EXCEPTION_CHROMOSOME_LENGTH;
}


/* */

/*
	Metoda koja racanu dobrotu jedinke
	param: jedinka, mreza, ulazni skup
	retur: double vrijednost dobrote
*/
double GA::calcFitness(ptrIndividual &ind, shared_ptr<NeuralNetwork> &nn, 
						 shared_ptr<DataSet> &in) 
{
	vector<double> p = ind->getChromosome(); // ind vraca &
	double err = nn->calcError(p, in);
	return err;
}



/*
	Konstruktor koji postavlja selekciju
	param: k-turnira, brojac
*/
Selection::Selection(int _k): k(_k) { }


/*
	Metoda koja radi selekciju metodom k-turnira
	param: vektor populacije
*/
int Selection::select(vPopulation &pop) {
	int ret = rand() % pop.size();
	for(int i = 1; i < k; i++) {
		int index = rand() % pop.size();
		if(pop.at(index)->getFitness() < pop.at(ret)->getFitness())
			ret = index;
	}

	return ret;
}



/*
	Metoda koja krizanje temeljeno na aritmetickoj sredini
*/
void Arithmetic::crossover(const ptrIndividual &f, const ptrIndividual &s, ptrIndividual &n) {
	for(int i = 0; i < f->getSize(); i++) {
		double rez = f->getComponent(i) + s->getComponent(i);
		n->updateComponent(i, rez/2);
	}
}



/*
	Metoda koja radi krizanje s jednom tockom prekida
*/
void TockaPrekida::crossover(const ptrIndividual &f, const ptrIndividual &s, ptrIndividual &n) {

	int cut = rand() % f->getSize();
	for(int i = 0; i < f->getSize(); i++) {
		if ( i < cut )
			n->updateComponent(i, f->getComponent(i));
		else 
			n->updateComponent(i, s->getComponent(i));
	}
}



/*
	Metoda koja radi uniformno krizanje
*/
void Uniform::crossover(const ptrIndividual &f, const ptrIndividual &s, ptrIndividual &n) {

	for(int i = 0; i < f->getSize(); i++) {
		if (rand() % 2 == 0)
			n->updateComponent(i, f->getComponent(i));
		else 
			n->updateComponent(i, s->getComponent(i));
	}
}


/*
	Konstruktor operatora mutacije1
	param: devijacija, vjerojatnost mutacije gena
*/
Mutation1::Mutation1(double _sigma, double _v, std::mt19937 g) : sigma(_sigma), v(_v), generator(g) { }


/*
	Metoda koja implementira operator mutacije 
	komponenti dodaje slucajno generiranu vrijednost iz gaussove razdiobe(0, sigma)
	param: jedinka
*/
void Mutation1::mutate(ptrIndividual &i) {
	std::normal_distribution<double> gauss(0.0, sigma);
	int N = i->getSize();

	for(int j = 0; j < N; j++) {
		double R = (double)rand()/RAND_MAX;
		if(R < v ) {
			double current = i->getComponent(j);
			double m = gauss(generator);
			i->updateComponent(j, m + current);
		}
	}
}


/*
	Konstruktor operatora mutacije2
	param: devijacija, vjerojatnost mutacije gena
*/
Mutation2::Mutation2(double _sigma, double _v, std::mt19937 g) : sigma(_sigma), v(_v), generator(g) { }


/*
	Metoda koja implementira operator mutacije 
	komponentu zamjeni slucajno generiranom vrijednosti iz gaussove razdiobe(0, sigma)
	param: jedinka
*/
void Mutation2::mutate(ptrIndividual &i) {
	std::normal_distribution<double> gauss(0.0, sigma);

	int N = i->getSize();
	for(int j = 0; j < N; j++) {
		double R = (double)rand()/RAND_MAX;
		if(R < v) {
			double m = gauss(generator);
			i->updateComponent(j, m);
		}
	}
}
