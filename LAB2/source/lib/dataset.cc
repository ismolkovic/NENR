#include "dataset.h"

/* 
	Konstruktor
 	param: inFile - ulazna datoteka s uzorcima
*/
DataSet::DataSet(const char *inFile) {
	ifstream input(inFile, ios::in);
	if(!input.is_open()) throw EXCEPTION_OPEN_FILE;

	string buffer;
	while(getline(input, buffer)) {
		SAMPLE s = parseLine(buffer);
		_dataSet.push_back(s);
	}
}


/*
	Metoda koja parsira liniju datoteke s uzorcima
	return: SAMPLE vektor
	param: string linija za parsiranje
*/
SAMPLE DataSet::parseLine(string line) {
	SAMPLE ret; double val;
	istringstream stream(line);

	while(stream >> val) {
		ret.push_back(val);
	}

	int check = getCount() == 0 ? 0 : getSample(0).size() != ret.size();
	if (check) throw EXCEPTION_SAMPLE_LENGTH; 

	return ret;
}


/*
	Metoda koja vraca odredeni uzorak
	param: i - index trazenog uzorka
*/
SAMPLE DataSet::getSample(int i) {
	if( i < getCount() && i >= 0)
		return _dataSet.at(i);
	throw EXCEPTION_INVALID_INDEX;
}


/*
	Metoda koja vraca broj uzoraka u dataSetu
*/
long DataSet::getCount() {
	return _dataSet.size();
}


/* 
	Preopteraceni operator << za ispisivanje SAMPLE vektora
*/
std::ostream& operator<<(std::ostream& out, const vector<double>& s) {
	for(unsigned int i = 0; i < s.size(); i++) {
		if( i != 0) out << " ";
		out << s.at(i);
	}
	return out;
}