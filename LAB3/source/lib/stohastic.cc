#include "stohastic.h"

StohasticGD::StohasticGD(int _N, Data *in) : N(_N), input(in) {
	srand (time(NULL));
	p = generateRandom(N); q = generateRandom(N); r = generateRandom(N);
	a = generateRandom(2 * N); b = generateRandom(2 * N); c = generateRandom(2 * N);
	w = generateRandom(N); f = generateRandom(N);
	miA = generateRandom(N); miB = generateRandom(N);

	outPtr = new Output(); fuzzyPtr = new FuzzySet();
	antPtr = new Antecedent(); conPtr = new Consequent();
	derPtr = new Derivation();
}

StohasticGD::~StohasticGD() {
	delete outPtr; delete fuzzyPtr; delete antPtr; 
	delete conPtr; delete derPtr;
}


vector<double> StohasticGD::generateRandom(int N) {
	vector<double> ret;

	for(int i = 0; i < N; i++)
		ret.push_back(((double)rand() / RAND_MAX)  * 10- 3);
	return ret;
}

void StohasticGD::generateWF(double& x, double& y, double& sumWj) {
	sumWj = 0;	
	for(int m = 0; m < N; m++) {
		f.at(m) = conPtr->getF(p.at(m), q.at(m), r.at(m), x, y);
		miA.at(m) = fuzzyPtr->getMembership(x, a.at(m * 2), b.at(m*2), c.at(m*2));
		miB.at(m) = fuzzyPtr->getMembership(y, a.at(m * 2+1), b.at(m*2+1), c.at(m*2+1));
		w.at(m) = antPtr->getW(miA.at(m), miB.at(m));
		sumWj += w.at(m);
	}
}

void StohasticGD::updateParams(double& dE_dOk, double& x, double& y, double& eta, double& sumWj) {
	for(int m = 0; m < N; m++) {
		double A = dE_dOk * derPtr->dOk_dWi(w, f, m, sumWj);
		double B = A * derPtr->dWi_dmib(miA.at(m), miB.at(m));
		A = A * derPtr->dWi_dmia(miA.at(m), miB.at(m));

		double daA = A * derPtr->dmi_da(a.at(m * 2), b.at(m *2), c.at(m * 2), x);
		double dbA = A * derPtr->dmi_db(a.at(m * 2), b.at(m *2), c.at(m * 2), x);
		double dcA = A * derPtr->dmi_dc(a.at(m * 2), b.at(m * 2), c.at(m * 2), x);

		double daB = B * derPtr->dmi_da(a.at(m * 2  + 1), b.at(m *2 + 1), c.at(m * 2 + 1), y);
		double dbB = B * derPtr->dmi_db(a.at(m * 2  + 1), b.at(m *2 + 1), c.at(m * 2 + 1), y);
		double dcB = B * derPtr->dmi_dc(a.at(m * 2  + 1), b.at(m * 2  + 1), c.at(m * 2 + 1), y);

		// promjena parametara
		p.at(m) -= eta * derPtr->getDeltaP(dE_dOk, w.at(m), sumWj, x);
		q.at(m) -= eta * derPtr->getDeltaQ(dE_dOk, w.at(m), sumWj, y);
		r.at(m) -= eta * derPtr->getDeltaR(dE_dOk, w.at(m), sumWj);

		a.at(m * 2) -= eta * daA;  	   
		b.at(m * 2) -= eta * dbA;  	  
		c.at(m * 2) -= eta * dcA;
		a.at(m * 2 + 1) -= eta * daB;  
		b.at(m * 2 + 1) -= eta * dbB;  
		c.at(m * 2 + 1) -= eta * dcB;
	}
}


void StohasticGD::run(double eps, int iter, double eta) {
	double x, y, z, sumWj;
	while(iter > 0) {
		double maxE = 0;

		for(int i = 0; i < input->getSize(); i++) {	
			x = input->getX(i); 
			y = input->getY(i);
			
			generateWF(x, y, sumWj);
			double o = outPtr->calcOutput(w, f); z = input->getZ(i);
			double dE_dOk = -(z-o);

			updateParams(dE_dOk, x, y, eta, sumWj);
			maxE += (z - o) * (z - o);
		}
		
		maxE = maxE / input->getSize();
		if(maxE < eps) break;

		if ( iter % 1000 == 0){
			cout << "Greska: " <<  maxE << endl; 
		}

		iter--;
	}
}

void StohasticGD::print() {
	
	for(int i = 0; i < input->getSize(); i++) {
		double x = input->getX(i);
		double y = input->getY(i);
		double sumWj = 0;	

		for(int m = 0; m < N; m++) {
			f.at(m) = conPtr->getF(p.at(m), q.at(m), r.at(m), x, y);
			miA.at(m) = fuzzyPtr->getMembership(x, a.at(m * 2), b.at(m*2), c.at(m*2));
			miB.at(m) = fuzzyPtr->getMembership(y, a.at(m * 2+1), b.at(m*2+1), c.at(m*2+1));
			w.at(m) = antPtr->getW(miA.at(m), miB.at(m));
			sumWj += w.at(m);
		}

		double o = outPtr->calcOutput(w, f);
		double z = input->getZ(i);
		cout << x << " " << y << " " << z << " " << o << endl;
	}
}
