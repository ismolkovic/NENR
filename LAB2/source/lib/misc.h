/*
	Datoteke koja sadrži tipove podataka 
	te includa potrebne biblioteke
	definira exceptione za pojedina ponasanja	
*/

#ifndef MISC
#define MISC 


#include <vector>
#include <string>
#include <memory>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <climits>
#include <random>

using namespace std;

typedef vector< double > SAMPLE;

#define RED  "\033[22;31m"
#define GREEN "\033[32m"
#define RESET_COLOR "\e[m"

#define EXCEPTION_INVALID_INDEX string("Invalid index!!!")
#define EXCEPTION_OPEN_FILE string("Can't open file!!!")
#define EXCEPTION_SAMPLE_LENGTH string("Invalid sample length!!!")
#define EXCEPTION_DEF_NN string("Invalid definition of nn!!!")
#define EXCEPTION_VECTOR_RANGE string("Vector out of range!!!")
#define EXCEPTION_PARAMS string("Too many or too less parameters!!!")
#define EXCEPTION_CHROMOSOME_LENGTH string("Chromosome out of range!!!")

#endif