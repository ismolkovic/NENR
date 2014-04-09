#ifndef FUNC_H
#define FUNC_H 

#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <deque>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <vector>	
#include <cstdlib>
#include <cstdio>
#include <boost/any.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>    
#include <boost/range/algorithm/remove_if.hpp>

using boost::is_any_of;
using boost::any_cast;
using namespace std;

typedef boost::any Object;
typedef vector< Object > vectorObject;
typedef pair< string, string > PAIRS;

class node;
class leaf;
class operation;
class App;
class Info;
class Domain;
class FuzzySet;
class Rules;
typedef vector< Domain* > DomainVector;


#define D_CARTESIAN string("cartesian")
#define D_REAL string("real")
#define D_INTEGER string("integer")
#define D_ENUMERATED string("enumerated")

#define F_GAMMA string("gamma")
#define F_LAMBDA string("lambda")
#define F_L string("l")
#define F_PI string("pi")

#define DEFAULT_T string("ZadehT")
#define DEFAULT_S string("ZadehS")
#define DEFAULT_NOT string("ZadehNot")
#define HAMMACHER_T string("HammacherT")
#define HAMMACHER_S string("HammacherS")
#define MAMDANI string("->")
#define NOT_DEFINED double(-1)
#define MAX_MIN string("max-min")
#define MAX_PRODUCT string("max-product")


string int2string(int);
string double2string(double);
double string2double(string);
int string2int(string);

string ObjectPair2string(Object);
int ObjectPair2int(Object);

int Object2int(Object);
double Object2double(Object);
string Object2string(Object);
vectorObject Object2vectorObject(Object);
Object vObject2Object(vectorObject);

Object pairObject(Object, Object);

string Domain2string(Domain*);

std::ostream& operator<<(std::ostream& out, const Object& o);
void printObject(ostream&, const Object&);

vector<string> splitString(string,string);
string removeSpaces(string);
string removeBrackets(string);
string begin2first_delimiter(string, string);
string cutSubString(string,string,string);
vector< string > getFuzzyPairsString(string);
vector< PAIRS > getFuzzyPairs(vector<string>&);
bool isBracket(char);
bool findOperator(string input);
string removeAlpha(string);

bool operator<(const Object&, const Object&);
bool operator>(const Object&, const Object&);
bool operator<=(const Object&, const Object&);
bool operator>=(const Object&, const Object&);

double operator-(const Object&, const Object&);
bool compareAtom(const Object&, const Object&);
bool operator==(const Object&, const Object&);
double operator*(double, const Object&);

#endif
