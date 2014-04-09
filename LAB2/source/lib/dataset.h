#ifndef DATA_SET_H
#define DATA_SET_H

#include "misc.h"

/*
	Klasa koja implementira funcionalnosti nad ulaznim skupom podataka.
		- dohvat određenog uzorka 
		- dohvat broja uzoraka u skupu
*/
class DataSet {
private:
	vector< SAMPLE > _dataSet;
	SAMPLE parseLine(string);

public:
	DataSet(const char*);
	~DataSet() { }

	SAMPLE getSample(int);
	long getCount();
};

std::ostream& operator<<(std::ostream& out, const SAMPLE& s);


#endif