#include "factory.h"

// Factory 
// factory metode
namespace Factory {

	Domain* domainFactory(string type, string inStr) {
		if (type == D_ENUMERATED) {
			inStr = inStr.substr(1, inStr.size() - 2);
			vector< string > vals = splitString(removeSpaces(inStr),",");

			return new enumerated(vals); 
		}

		if (type == D_INTEGER) {
			vector<string> vals = splitString(inStr, " ");
			
			int lower = string2int(vals[ 0 ]);
			int upper = string2int(vals[ 2 ]);
			int step = string2int(vals[ 4 ]);

			return new integer(lower, upper, step); 
		}

		if(type == D_REAL) {
			vector<string> vals = splitString(inStr, " ");
			
			double lower = string2double(vals[ 0 ]);
			double upper = string2double(vals[ 2 ]);
			double step = string2double(vals[ 4 ]);

			return new real(lower, upper, step); 
		}

		if (type == D_CARTESIAN) {
			vector<string> domString = splitString(removeSpaces(inStr), ",");
			
			vector< Domain* > vDom;
			for(unsigned int i = 0; i < domString.size(); i++) 
				vDom.push_back(Info::Instance()->getDomainPointer(domString[ i ]));

			return new cartesian(vDom);
		}

		return NULL;
	}



	FuzzySet* createSimpleSet(string d, vector< string >& input) {
		vector< PAIRS > _pairs = getFuzzyPairs(input);

		return new SimpleSet(Info::Instance()->getDomainPointer(d), _pairs);
	}


	FuzzySet* fuzzySetFactory(string d, string f, vector< string >& input) {
		vector< Object > param; 
		Domain *dom = Info::Instance()->getDomainPointer(d);

		for(unsigned int i = 0; i < input.size(); i++) {
			param.push_back(dom->fromStringRepresentation(input[ i ]));
		}

		if (f == F_GAMMA) {
			return new Gamma(dom, param.at(0), param.at(1));
		}

		if (f == F_LAMBDA) {
			return new Lambda(dom, param.at(0), param.at(1), param.at(2));
		}

		if (f == F_L) {
			return new L(dom, param.at(0), param.at(1));
		}

		if (f == F_PI)  {
			return new Pi(dom, param.at(0), param.at(1), param.at(2),
				param.at(3));
		} 

		return NULL;
	}


	FuzzySet** operatorFactory(string o, FuzzySet **first, FuzzySet **second) {
		// tu sad imamo ZadehNot, ZadehS, ZadehT, HammacherT, HammacherS i Mamdani
		try {
			string operator_ = Info::Instance()->getOperator(o);
			FuzzySet **ret = new FuzzySet*;

			if(operator_ == DEFAULT_T) {
				*ret = new ZadehT(first, second);
			}

			else if(operator_ == DEFAULT_S) {
				*ret = new ZadehS(first, second);
			}

			else if(operator_ == DEFAULT_NOT) {
				*ret = new ZadehNot(first);
			}

			else if(operator_.find(HAMMACHER_T) != string::npos) {
				double par = string2double(cutSubString(operator_, "(", ")"));
				*ret = new HammacherT(par, first, second);
			}

			else if(operator_.find(HAMMACHER_S) != string::npos) {
				double par = string2double(cutSubString(operator_, "(", ")"));
				*ret = new HammacherS(par, first, second);
			}

			else if(operator_.find(MAMDANI) != string::npos) {
				vector< Domain* > dVec; 
				dVec.push_back((*first)->getDomain());
				dVec.push_back((*second)->getDomain());

				Domain *d = new cartesian(dVec);
				string name = Domain2string(d);

				string d1Str = Info::Instance()->getDomainStringFromPointer((*first)->getDomain());
				string d2Str = Info::Instance()->getDomainStringFromPointer((*second)->getDomain());

				pair< string, string > type = make_pair("cartesian", d1Str + "," + d2Str);
				Info::Instance()->registerDomain(name, d, type);

				*ret = new Mamdani(d, first, second);
			}

			else  throw 12; 

			Info::Instance()->addTreeFuzzySet(ret);
			return ret;
		}
		catch (int e) { 
			cerr << "Operator " << o << " ne postoji." << endl;
			return NULL;
		}
	}


	FuzzySet* createExpr(FuzzySet** f) {
		return new Expr((*f)->getDomain(), f);
	}


	FuzzySet* createProject(FuzzySet* fuz, Domain *dom) {
		
		string domain1 = Info::Instance()->getDomainStringFromPointer(fuz->getDomain());
		string domain2 = Info::Instance()->getDomainStringFromPointer(dom);

		// bacit ce iznimku ako domena2 nije podskup od domene1
		vector< int > pos = Info::Instance()->findProjectExtensionPositions(domain1, domain2);			
		return new Project(fuz, dom, pos);
	}


	FuzzySet* createCylindricalExtension(FuzzySet *fuz, Domain *dom) {
		string domain1 = Info::Instance()->getDomainStringFromPointer(fuz->getDomain());
		string domain2 = Info::Instance()->getDomainStringFromPointer(dom);

		vector< int > pos = Info::Instance()->findProjectExtensionPositions(domain2, domain1);

		return new CylindricalExtension(fuz, dom, pos);
	}

	FuzzySet* createComposition(FuzzySet *f1, FuzzySet *f2, string type) {
		string domain1 = Info::Instance()->getDomainStringFromPointer(f1->getDomain());
		string domain2 = Info::Instance()->getDomainStringFromPointer(f2->getDomain());

		vector < int > pos = Info::Instance()->findChainPositions(domain1, domain2);
		if(pos.size() == 0) throw 20;

		string domainString = createCompositionDomainString(f1, f2, pos);

		Domain *out = createCompositionDomain(domainString);
		return new Composition(f1, f2, type, out, pos);
	}


	string createCompositionDomainString(FuzzySet *f, FuzzySet *s, vector<int>& pos) {
		string d1 = Info::Instance()->getDomainStringFromPointer(f->getDomain());
		string d2 = Info::Instance()->getDomainStringFromPointer(s->getDomain());

		vector< string > domain1 = splitString(d1, ",");
		vector< string > domain2 = splitString(d2, ",");

		string val;
		
		for(int i = 0; i < (int)domain1.size(); i++) {
			if(val.empty() && pos.at(i) == 0) val = domain1.at(i);
			else if (pos.at(i) == 0) val += "," + domain1.at(i);
		}

		
		int N = domain1.size();
		for(int i = N; i < (int)domain2.size() + N; i++) {
			if(pos.at(i) == 0 && val.empty()) val = domain2.at(i - N);
			else if(pos.at(i) == 0) val += "," +  domain2.at(i - N);
		}
	
		return val;
	}

	 Domain* createCompositionDomain(string dom) {
	 	if( dom.find(",") == string::npos) 
	 		return Info::Instance()->getDomainPointer(dom);

	 	Domain *out = domainFactory(D_CARTESIAN, dom);
	 	string name = Domain2string(out);
	 	Info::Instance()->registerDomain(name, out, make_pair(D_CARTESIAN, dom));

	 	return out;
	 }


}