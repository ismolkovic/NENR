#include "nn.h"

/*
	Kontruktor NeuralNetwork
	param: vektor s informacijama o arhitekturi slojeva
*/
NeuralNetwork::NeuralNetwork(vector<int> &info) : _info(info) {
	_neuronsNumber = _paramNumber = 0;
	
	if(getNumberOfLayers() < 3) throw EXCEPTION_DEF_NN;

	_paramNumber += 2 * getSizeOfLayer(0) * getSizeOfLayer(1);
	for(unsigned int i = 0; i < getNumberOfLayers(); i++) {
		_neuronsNumber += getSizeOfLayer(i);
		if(i > 1) _paramNumber += getSizeOfLayer(i) * (getSizeOfLayer(i-1) + 1);
	}
	
	_output.resize(_neuronsNumber, 0);
}


/*
	Metoda koja vraca ukupan broj neurona
*/
long NeuralNetwork::getNumberOfNeurons() {
	return _neuronsNumber;
}


/*
	Metoda koja vraca ukupan broj parametara
*/
long NeuralNetwork::getNumberOfParameters() {
	return _paramNumber;
}

/*
	Metoda koja vraca broj neurona u pojedinom sloju
	param: index sloja
*/
long NeuralNetwork::getSizeOfLayer(int l) {
	if (l < (signed)_info.size()) return _info.at(l);
	throw EXCEPTION_INVALID_INDEX;
}

/*
	Metoda koja vraca broj slojeva mreze
*/
long NeuralNetwork::getNumberOfLayers() {
	return _info.size();
}

/*
	Metoda koja racuna izlaz mreze 
	param: vektor parametara, ulaz 
	return: vraca vrijednosti izlaza u izlaznom sloju
*/
vector< double > NeuralNetwork::calcOutput(vector<double> &params, SAMPLE &input) {
	vector< double > ret;
	if((signed)params.size() != getNumberOfParameters()) throw EXCEPTION_PARAMS;

	int lenLayer1 = getSizeOfLayer(0);
	for(int i = 0; i < lenLayer1; i++) 
		_output[ i ] = input.at(i);
	
	int startP = 0; int lenLayer2 = getSizeOfLayer(1);
	for(int i = lenLayer1; i < lenLayer1 + lenLayer2; i++) {
		_output[ i ] = getType1Output(params, _output, startP, lenLayer1, 0);
		startP += 2 * lenLayer1; 
	}

	int l = 2; int startI = lenLayer1; int j = 0;
	for(int i = lenLayer2 + lenLayer1; i < getNumberOfNeurons(); i++) {
		int n  = getSizeOfLayer(l); int Neur = getSizeOfLayer(l-1);

		_output[ i ] = getType2Output(params, _output, startP, Neur, startI);
		startP += Neur + 1; 

		j++;
		if(j == n) { startI += getSizeOfLayer(l-1); j = 0; l++; } 
	}

	int N = getNumberOfNeurons();
	for(int i = N - getSizeOfLayer(getNumberOfLayers()-1); i < N; i++) 
		ret.push_back(_output.at(i));

	return ret;
}


/*
	Metoda koja računa ukupnu srednju kvadratnu pogrešku
	param: polje parametera, i referenca na DataSet
*/
double NeuralNetwork::calcError(vector<double> &params, shared_ptr<DataSet> &dataSet) {
	double err = 0;
	int start = getSizeOfLayer(0);
	int N = dataSet->getCount();
	
	for(int i = 0; i < N; i++) {
		SAMPLE s = dataSet->getSample(i);
		vector< double > out = calcOutput(params, s);
		int M = out.size();
		
		if((signed)s.size() != start + M) throw EXCEPTION_VECTOR_RANGE;
		for(int j  = 0; j < M; j++) {
			err += (out.at(j) - s.at(j + start)) * (out.at(j) - s.at(j + start));
		}
	}

	return err / N;
}	


/*
	Metoda koja racuna izlaz TIP 1 neurona
	params: polje parametara, ulaz,
			  pocetak u polju parametara i broj neurona na ulazu za koje uzimamo parametre
	 		  te pocetak u polju ulaza
	return: vraca vrijednost izracunatoj po formuli

	Ocekuje da je prvi parametar Wi, a drugi Si
*/
double NeuralNetwork::getType1Output(vector<double> &params, vector<double> &input, 
				int startP, int n, int startI) 
{
	double sum = 0;
	for(int i = 0 ; i < n; i++) {
		double S = params.at(startP + i*2 + 1);
		if ( S == 0.f ) continue;
		sum += fabs(input.at(i + startI) - params.at(startP + i*2)) / fabs(S);
	}

	return (double)1.0f / (1.0f + sum);
}


/*
	Metoda koja racuna izlaz TIP 2 neurona
	params: polje parametara, ulaz, pocetak u polju neurona, broj neurona koji su na ulazu,
			  pocetak u polju ulaza
	return: vraca izlaz sigmoide

	Ocekuje parametre w0, w1, w2, ..., wn-1
*/
double NeuralNetwork::getType2Output(vector<double> &params, vector<double> &input, 
				int startP, int n, int startI) 
{
	double sum = params.at(startP);
	for(int i = 0; i < n; i++) {
		sum += params.at(startP + i + 1) *  input.at(startI + i);
	}

	return (double)1.0f / (1.0f + exp(-sum));
}


/*
	Metoda koja postavlja vrijedosti 0 ili 1 za izlaze
	Ako je >=0.5 onda 1, inace 0

	params: vektor vrijednosti
	return: vektor 0, 1 vrijednosti
*/
vector< double > NeuralNetwork::decodeOutput(vector<double>& in) {
	vector< double > ret;
	for(unsigned int i = 0; i < in.size(); i++) {
		if(in.at(i) >= 0.5) ret.push_back(1);
		else ret.push_back(0);
	}

	return ret;
}


/*
	Metoda koja racuna izlaze za sve uzorke, prikazuje dobiveni i tocan izlaz
	te ispisuje koliko ima pogresno i tocno klasificiranih
	param: polje parametara, skup za testiranje
*/
void NeuralNetwork::printSolution(vector<double> &Param, shared_ptr<DataSet> &In) {
	cout << "Tip 1 neurona: " << endl;
	cout << "Wi: " << endl;
	for(int i = 0; i < 4 * getSizeOfLayer(1); i += 4)
		cout <<  Param.at(i) << " " << Param.at(i + 2) << endl;


	cout << endl << "Si:" << endl;
	for(int i = 0; i < 4 * getSizeOfLayer(1); i+= 4) {
		cout <<  Param.at(i+1) << " " << Param.at(i + 3) << endl;
	}

	cout << endl << "Ostale tezine" << endl;
	for(int i = 4 * getSizeOfLayer(1); i < getNumberOfParameters(); i++)
		cout << Param.at(i) << endl;


	cout << endl <<  "Dobiveni izlazi: " << endl;


	int N = In->getCount();
	int P = 0;
	for(int i = 0; i < N; i++) {
		SAMPLE s = In->getSample(i);
		vector< double > out = calcOutput(Param, s);
		out = decodeOutput(out);

		int M = s.size() - out.size();
		cout <<"\t";
		for(int i = 0; i < M; i++) printf("%.8lf ", s.at(i));
		cout << "	[ ";
		for(int i = M; i < (signed)s.size(); i++) cout << s.at(i) << " ";
		cout << "]  ->  [ " << out << " ]";

		if(checkOutput(s, out)) {
			P++;
			printf(GREEN " T" RESET_COLOR "\n");
		}
		else printf(RED " F" RESET_COLOR "\n");
	}

	cout << endl <<"Konačna greška: " << calcError(Param, In);
	cout << endl; printf(GREEN "True: " RESET_COLOR); cout << P << " / " << N << endl;
	printf(RED "False: " RESET_COLOR);  cout << N - P << " / " << N << endl;
}


/*
	Metoda koja provjerava dobiveni izlaz s predvidenim
	param: uzorak, izlaz
	return: true isti su inace false
*/
bool NeuralNetwork::checkOutput(vector<double> &f, vector<double> &s) {
	int M = f.size() - s.size();
	for(int j = M ; j < (signed)f.size(); j++) {
		if(f.at(j) != s.at(j - M)) return false;
	}
	return true;
}