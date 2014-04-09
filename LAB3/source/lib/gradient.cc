#include "gradient.h"

BatchGD::BatchGD(int _N, Data* in) : N(_N), input(in) {
	srand (time(NULL));
	p = generateRandom(N); q = generateRandom(N); r = generateRandom(N);
	a = generateRandom(2 * N); b = generateRandom(2 * N); c = generateRandom(2 * N);
	w = generateRandom(N); f = generateRandom(N);
	miA = generateRandom(N); miB = generateRandom(N);

	outPtr = new Output(); fuzzyPtr = new FuzzySet();
	antPtr = new Antecedent(); conPtr = new Consequent();
	derPtr = new Derivation();
}

BatchGD::~BatchGD() {
	delete outPtr; delete fuzzyPtr; delete antPtr; 
	delete conPtr; delete derPtr;
}

vector<double> BatchGD::generateRandom(int N) {
	vector<double> ret;
	for(int i = 0; i < N; i++)
		ret.push_back(((double)rand() / RAND_MAX)  * 10 -5);
	return ret;
}

void BatchGD::generateWF(double& x, double& y, double& sumWj) {
	sumWj = 0;	
	for(int m = 0; m < N; m++) {
		f.at(m) = conPtr->getF(p.at(m), q.at(m), r.at(m), x, y);
		miA.at(m) = fuzzyPtr->getMembership(x, a.at(m * 2), b.at(m*2), c.at(m*2));
		miB.at(m) = fuzzyPtr->getMembership(y, a.at(m * 2+1), b.at(m*2+1), c.at(m*2+1));
		w.at(m) = antPtr->getW(miA.at(m), miB.at(m));
		sumWj += w.at(m);
	}
}

	
void BatchGD::run(double eps, int iter, double eta) {
	double x, y, z, sumWj;
	while(iter > 0) {
		vector< double >dp(N, 0), dq(N, 0), dr(N, 0);
		vector< double >da(2 * N, 0), db(2 * N, 0), dc(2 * N, 0);

		double maxE = 0;

		for(int i = 0; i < input->getSize(); i++) {	
			x = input->getX(i); 
			y = input->getY(i);
			z = input->getZ(i);

			generateWF(x, y, sumWj);
			double o = outPtr->calcOutput(w, f); z = input->getZ(i);
			double dE_dOk = -(z-o);

			maxE += (z - o) * (z - o);

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

				dp.at(m) += derPtr->getDeltaP(dE_dOk, w.at(m), sumWj, x);
				dq.at(m) += derPtr->getDeltaQ(dE_dOk, w.at(m), sumWj, y);
				dr.at(m) += derPtr->getDeltaR(dE_dOk, w.at(m), sumWj);

				da.at(m * 2) += daA;  	   
				db.at(m * 2) += dbA;  	  
				dc.at(m * 2) += dcA;
				da.at(m * 2 + 1) += daB;  
				db.at(m * 2 + 1) += dbB;  
				dc.at(m * 2 + 1) += dcB;
			}

		}

		// tu sad ide promjena parametara
		for(int m = 0; m < N; m++) {
			p.at(m) -= eta * dp.at(m);
			q.at(m) -= eta * dq.at(m);
			r.at(m) -= eta * dr.at(m);

			a.at(2 * m) -= eta * da.at(2 * m);
			a.at(2 * m + 1) -= eta * da.at(2 * m + 1);
			b.at(2 * m) -= eta * db.at(2 * m);
			b.at(2 * m + 1) -= eta * db.at(2 * m + 1);
			c.at(2 * m) -= eta * dc.at(2 * m);
			c.at(2 * m + 1) -= eta * dc.at(2 * m + 1);
		}

		maxE = maxE / input->getSize();
		if(maxE < eps) break;

		if ( iter % 1000 == 0){
			cout << "Greska: " <<  maxE << endl; 
		}
		
		iter--;
	}
}

void BatchGD::print() {
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

