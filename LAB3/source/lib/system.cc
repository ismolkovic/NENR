#include "system.h"


double FuzzySet::getMembership(double& x, double& a, double& b, double& c) {
	if( a == 0) return 0;
	double v = (x - c)/a; v = v * v;
	v = pow(v, b);

	return exp(-v); 
}


double Antecedent::getW(double& miA, double& miB) {
	double v = miA * miB;
	return v / (2 - (miA + miB - miA * miB));
}


double Consequent::getF(double& p, double& q, double& r, double& x, double& y) {
	return p * x + q * y + r;
}


double Output::calcOutput(vector<double>& w, vector<double>& f) {
	if(w.size() != f.size()) throw string("ERROR!!!");
	double sumWF = 0;
	double sumW = 0;
	for(unsigned int i = 0; i < f.size(); i++) {
		sumWF += w.at(i) * f.at(i);
		sumW += w.at(i);
	}

	if(sumW == 0) return 0;
	return sumWF / sumW;
}

Data::Data(const char* file) {
	size = 0;
	FILE *f = fopen(file, "r");
	double xval, yval, zval;

	while(fscanf(f, "%lf %lf %lf", &xval, &yval, &zval) > 0) {
		x.push_back(xval);
		y.push_back(yval);
		z.push_back(zval);
		size++;
	}

	fclose(f);
}
	
double Data::getX(int i) {
	return x.at(i);
}

double Data::getY(int i) {
	return y.at(i);
}
	
double Data::getZ(int i) {
	return z.at(i);
}

long Data::getSize() {
	return size;
}


double Derivation::getDeltaP(double& dE_dOk, double& wi, double& sumWj, double& x) {
	if(dE_dOk == 0 || wi == 0 || x == 0) return 0;
	return dE_dOk * wi * x / sumWj;
}

double Derivation::getDeltaQ(double& dE_dOk, double& wi, double& sumWj, double& y) {
	if(dE_dOk == 0 || wi == 0 || y == 0) return 0;
	return dE_dOk * wi * y / sumWj;
}

double Derivation::getDeltaR(double& dE_dOk, double& wi, double& sumWj) {
	if(dE_dOk == 0 || wi == 0) return 0;
	return dE_dOk * wi / sumWj;
}

double Derivation::dWi_dmia(double& miA, double& miB) {
	double u = -miB *( miB - 2 );
	double v = (2 - (miA + miB - miA*miB));

	return u / (v * v);
}

double Derivation::dWi_dmib(double& miA, double& miB) {
	double u = -miA *( miA - 2 );
	double v = (2 - (miA + miB - miA*miB));

	return u / (v * v);
}

double Derivation::dOk_dWi(vector<double>& w, vector<double>& f, int& i, double& sumWj) {
	double sumWF = 0;
	for(int j = 0; j < (int)w.size(); j++) {
		if (i == j) continue;
		sumWF += w.at(j) * (f.at(i) - f.at(j));
	}

	if(sumWF == 0) return 0;
	return sumWF / (sumWj * sumWj);
}

double Derivation::dmi_da(double& a, double& b, double& c, double& x) {
	double u = 2 * b / a;
	double v = (x -c) /a ; v = v * v;  v = pow(v, b);

	return u * exp(-v) * v;
}

double Derivation::dmi_dc(double& a, double& b, double& c, double& x) {
	double u = 2 * b/(x-c);
	double v = (x -c) /a ; v = v * v;  v = pow(v, b);

	return u * exp(-v) * v;
}

double Derivation::dmi_db(double& a, double& b, double& c, double& x) {
	double v = (x -c) /a ; v = v * v;  double u = pow(v, b);

	return -exp(-u) * u * log(v);  
}