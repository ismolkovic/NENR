#include "func.h"

string int2string(int i) {
	ostringstream convert; string value;
	
	convert << i; value = convert.str();
	return value;
}

string double2string(double d) {
	ostringstream convert; string value;
	d = fabs(d) > 10E-10 ? d : 0.f;

	convert << d; value = convert.str();
	return value;
}

Object vObject2Object(vectorObject a) {
	if(a.size() > 1) return Object(a);
	else return a.at(0);
}

double string2double(string s) { return atof(s.c_str()); }
int string2int(string s) { return atoi(s.c_str()); }

string Domain2string(Domain *d) {
	ostringstream convert; string value;
	
	convert << d; value = convert.str();
	return value;
}

string ObjectPair2string(Object o) {
	pair<int,string> a = any_cast<pair< int,string> >(o);
	return a.second;
}

int ObjectPair2int(Object o) {
	pair<int,string> a = any_cast<pair< int,string> >(o);
	return a.first;
}


std::ostream& operator<<(std::ostream& out, const Object& i)
{
		if (i.type() == typeid(vector<Object>)) {
			vectorObject a = Object2vectorObject(i);

			out << "(";
			for(unsigned int j = 0 ; j < a.size(); j++) {
				printObject(out, a[ j ]);
				
				if ( j < a.size() - 1)
					out << ",";
			}
			out << ")";

		}

		else printObject(out, i);
    return out;
}

void printObject(ostream& out, const Object& i) {
	if( i.type() == typeid(int))
		out << Object2int(i);

	if( i.type() == typeid(double))
		out << Object2double(i);

	if( i.type() == typeid(string))
		out << Object2string(i);

	if( i.type() == typeid(pair<int,string>)) 
		out << ObjectPair2string(i);
}

vector<string> splitString(string input, string delimiter) {
	vector<string> temp;
	boost::algorithm::split(temp, input, is_any_of(delimiter));

	return temp;
}

string removeSpaces(string input) {
	input.erase(boost::remove_if(input, ::isspace), input.end());
	return input;
}

string begin2first_delimiter(string input, string delimiter) {
	size_t pos = input.find_first_of(delimiter);
	return input.substr(0, pos);
}

string cutSubString(string in, string first, string second) {
	size_t start = in.find(first);
	size_t end = in.find(second);

	if (start == string::npos || end == string::npos) return in;
	return in.substr(start + first.size(), end - start -1);
}


vector< string > getFuzzyPairsString(string in) {
	vector<string> ret;
	int start = 0; int cut = 0;
	for(unsigned int i = 1; i < in.length(); i++) {
		if ( in.at(i) == '(') cut += 1 ;
		if ( in.at(i) == ')') cut -= 1;
		if( cut == 0 && (in.at(i) == ',' || in.at(i) == '+')) {
			string value = removeBrackets(in.substr(start, i - start));
			ret.push_back(value); start = i + 1;
		}
	}
	ret.push_back(removeBrackets(in.substr(start)));
	return ret;
}

string removeBrackets(string input) {
	input.erase(boost::remove_if(input, isBracket), input.end());
	return input;
}

string removeAlpha(string input) {
	input.erase(boost::remove_if(input, ::isalpha), input.end());
	return input;
}

bool isBracket(char c) {
	return c == '(' || c == ')';
}

vector< PAIRS > getFuzzyPairs(vector<string>& input) {
	vector< PAIRS > ret;

	for(unsigned int i = 0; i < input.size(); i++) {
		vector<string> temp = splitString(input.at(i), "/");
		ret.push_back( make_pair(temp.at(0), temp.at(1)));
	}

	return ret;
}

int Object2int(Object o) { return any_cast<int>(o); }

double Object2double(Object o) { return any_cast<double>(o); }

string Object2string(Object o) { return any_cast<string>(o); }

vectorObject Object2vectorObject(Object o) { 
	if ( o.type() == typeid(vector<Object>))
		return any_cast< vector<Object> >(o);
	else {
		vector< Object > ret; ret.push_back(o);
		return ret;
	} 
}

bool operator<(const Object& a, const Object& b) {
	if(a.type() == typeid(int) && b.type() == typeid(int)) {
		return Object2int(a) < Object2int(b);
	}

	if(a.type() == typeid(double) && b.type() == typeid(double)) {
		return Object2double(a) < Object2double(b);
	}

	if(a.type() == typeid(string) && b.type() == typeid(string)) {
		return Object2string(a) < Object2string(b);
	}

	if(a.type() == typeid(pair<int,string>) && b.type() == typeid(pair<int,string>)) {
		return ObjectPair2int(a) < ObjectPair2int(b);
	}

	return false;
}


bool operator>(const Object& a, const Object& b) { return (b < a); }
bool operator<=(const Object& a, const Object& b) { return ! (a > b) ;}
bool operator>=(const Object& a, const Object& b) { return ! (a < b) ;}


double operator-(const Object& a, const Object& b) {
	if(a.type() == typeid(int) && b.type() == typeid(int)) {
		return Object2int(a) - Object2int(b);
	}

	if(a.type() == typeid(double) && b.type() == typeid(double)) {
		return Object2double(a) - Object2double(b);
	}

	if(a.type() == typeid(string) && b.type() == typeid(string)) {
		string aval = removeAlpha(Object2string(a));
		string bval = removeAlpha(Object2string(b));

		return atof(aval.c_str()) - atof(bval.c_str());
	}

	if(a.type() == typeid(pair<int,string>) && b.type() == typeid(pair<int,string>)) {
		return ObjectPair2int(a) - ObjectPair2int(b);
	}

	return 0;
}


double operator*(double a, const Object& b) {
	if( b.type() == typeid(double))
		return a * Object2double(b);

	if( b.type() == typeid(int))
		return a * Object2int(b);
	
	else return 0;
}


bool compareAtom(const Object& a, const Object& b) {
	if(a.type() == typeid(int) && b.type() == typeid(int)) {
		return Object2int(a) == Object2int(b);
	}

	else if(a.type() == typeid(double) && b.type() == typeid(double)) {
		return Object2double(a) == Object2double(b);
	}

	else if(a.type() == typeid(string) && b.type() == typeid(string)) {
		return Object2string(a) == Object2string(b);
	}

	if(a.type() == typeid(pair<int,string>) && b.type() == typeid(pair<int,string>)) {
		return ObjectPair2int(a) == ObjectPair2int(b);
	}

	else return false;
}

bool operator==(const Object& a, const Object& b) {
	if(a.type() == typeid(vector<Object>) && b.type() == typeid(vector<Object>)) 
	{
		vector<Object> c = Object2vectorObject(a);
		vector<Object> d = Object2vectorObject(b);
		if(c.size() != d.size()) return false;

		for(unsigned int i = 0; i < c.size(); i++) {
			if(compareAtom(c[ i ], d[ i ]) == false) return false;
		}
		return true;
	}
	else return compareAtom(a, b);
}

bool findOperator(string input) {
	if (input.find("!") != string::npos) return true;
	if (input.find("*") != string::npos) return true;
	if (input.find("+") != string::npos) return true;
	if (input.find("->") != string::npos) return true;

	return false;
}

Object pairObject(Object a, Object b) {
	vector< Object > ret;
	ret.push_back(a); ret.push_back(b);
	return Object(ret);
}