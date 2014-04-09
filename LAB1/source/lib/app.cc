#include "app.h"

vector< string > operators = { "*", "+", "->" };

void Parser::parse(const char *file) {
	ifstream in(file, ifstream::in);

	if(!in.is_open()) throw 22;	
	string buffer;	

	while(getline(in, buffer)) {
		if(buffer.size() == 1 || buffer[0] == '#') continue;

		else if(buffer.find("domain") != string::npos) parseDomain(buffer);
		else if(buffer.find("fuzzyset @") != string::npos) parseFuzzySet(buffer);
		else if(buffer.find("write") != string::npos) parseWrite(buffer); 	
		else if(buffer.find("set operator") != string::npos) parseOperator(buffer);
		else if(buffer.find("fuzzyset expr") != string::npos) parseExpression(buffer);
		else if(buffer.find("testSymmetric") != string::npos) parseSymmetric(buffer);
		else if(buffer.find("testReflexive") != string::npos) parseReflexive(buffer);
		else if(buffer.find("testTransitive") != string::npos) parseTransitive(buffer);
		else if(buffer.find("testFuzzyEquivalence") != string::npos) parseFuzzyEquivalence(buffer);
		else if(buffer.find("fuzzyset project") != string::npos) parseProject(buffer);
		else if(buffer.find("fuzzyset cylindrical_extension") != string::npos) {
			parseCylindrical_extension(buffer);
		}
		else if(buffer.find("fuzzyset composition") != string::npos) parseComposition(buffer);

	}

	in.close();
}

void Parser::parseWrite(string input) {
	string name = cutSubString(input, " ", ";");
	
	if(Info::Instance()->isFuzzySetRegistered(name)) {
		cout << name << ": ";
		Info::Instance()->getFuzzySetPointer(name)->write();
	}
}

// rad s domenama
void Parser::parseDomain(string input) {	
	string key = begin2first_delimiter(input, ":"); input = input.substr(key.size() + 9);
	string type = input.substr(0, input.find_first_of(" "));
	
	input = input.substr(input.find_first_of(" ") + 1);
	input = input.substr(0, input.find_first_of(";"));

	
	Domain *value = Factory::domainFactory(type, input);
	Info::Instance()->registerDomain(key, value, make_pair(type, input));
}

// rad s fuzzysetovima
void Parser::parseFuzzySet(string input) {
	string key = begin2first_delimiter(input, ":");
	string dom = removeSpaces(cutSubString(input, "@", "is")), def;

	// ako je skup definiran sa { ... }
	if(input.find("{") != string::npos) {
		def = removeSpaces(cutSubString(input, "{", "}"));
		vector< string > elements = getFuzzyPairsString(def);

		// provjeravamo ako vec postoji onda kod redefinicije
		// mora imati istu domenu novi skup
		if(Info::Instance()->isFuzzySetRegistered(key)) {
			Domain *f = Info::Instance()->getDomainPointer(dom);
			Domain *s = Info::Instance()->getDomainFromFuzzyId(key);
			if(Info::Instance()->cmpDomain(f,s) == false) { 
				cerr << "Domene su različite!" << endl;
				return; 
			}
		}

		FuzzySet *value = Factory::createSimpleSet(dom, elements);
		Info::Instance()->registerFuzzySet(key, value);
	}

	// inace je skup definara s gamma, lambda ...
	else {
		input = input.substr(input.find("is") + 2);
		def = removeSpaces(cutSubString(input, " ", ";"));
		string function = begin2first_delimiter(def, "(");
		vector<string> parameters = splitString(removeSpaces(cutSubString(input, "(", ")")), ",");
	

		// provjeravamo ako vec postoji onda kod redefinicije
		// mora imati istu domenu novi skup
		if(Info::Instance()->isFuzzySetRegistered(key)) {
			Domain *f = Info::Instance()->getDomainPointer(dom);
			Domain *s = Info::Instance()->getDomainFromFuzzyId(key);
			if(Info::Instance()->cmpDomain(f,s) == false) { 
				cerr << "Domene su različite!" << endl;
				return; 
			}
		}

		FuzzySet *value = Factory::fuzzySetFactory(dom, function, parameters);
		Info::Instance()->registerFuzzySet(key, value);
	}
}

void Parser::parseExpression(string input) {
	string key = begin2first_delimiter(input, ":");
	string def = input.substr(input.find("expr") + 4);
	def = removeSpaces(begin2first_delimiter(def, ";"));

	try {
		node *rootS = createTree(def);
		
		if ( rootS != NULL ) {
			//printTree(rootS, 0); cout << endl;
			FuzzySet **root = createOperatorTree(rootS);
			delete rootS; // ostalo je pobrisano u metodi createOperatorTree

			if(root != NULL) {
				FuzzySet *value = Factory::createExpr(root);
				Info::Instance()->registerFuzzySet(key, value);
			}
		}	
	}

	catch(int e) {
		if (e == 14) cerr << "GRESKA OPERATOR" << endl;
		if (e == 15) cerr << "GRESKA KRAJ" << endl;
		if (e == 16) cerr << "GRESKA NULA DULJINA" << endl;
		if (e == 17) cerr << "GRESKA RAZLICITE DOMENE: " << key << endl;
	}
}

void Parser::parseOperator(string input) {
	input = input.substr(input.find("operator") + 8);
	string opt = removeSpaces(begin2first_delimiter(input, "to"));
	input = input.substr(input.find("to") + 2);
	string value = cutSubString(input, " ", ";");

	// postavimo zapis u bazu
	Info::Instance()->registerOperator(opt, value);
}

// metoda koja kreira sintaksno stablo
// listovi su oznake skupova fxx
// ostali cvorovi su operatori
node* Parser::createTree(string input) {
	if(input.size() == 0) throw 16;

	if(input.at(0) == 'f' && findOperator(input) == false) {
		if(Info::Instance()->isFuzzySetRegistered(input))
			return new leaf(input);
		throw 16;
	}

	int depth = 0;
	int N = input.size();
	node *tree = NULL;

	for(int i = N -1; i >=0; i--) {
		if(input.at(i) == ')') depth += 1;
		else if (input.at(i) == '(') depth -= 1;
		else if (depth == 0) {
			for(unsigned int j = 0; j < operators.size(); j++) {
				int start = i - operators[ j ].size() + 1;
				if(start < 0) continue;

				if(operators[ j ] != input.substr(start, operators[j].size()))
					continue;
				
				if(tree != NULL) throw 14;
				else { // imamo operator *, +, ->
					node *left = createTree(input.substr(0, start));
					node *right = createTree(input.substr(i+1, N-1));

					tree = new operation(left, right, operators[ j ]);
					break; // izlazimo iz petlje operatora
				}	
			}
		}
	}

	if (tree != NULL) return tree;

  if (input.at(0) == '!') {
    node *Not = createTree(input.substr(1, N - 1));
    return new operation( Not, NULL, "!");
  }

  if (input.at(0) == '(' && input.at(N - 1) == ')') {
    return createTree(input.substr(1, N - 2));
  }

	throw 15;
}

// idemo u obilazak sintaksnog stabla i kreiramo operatorsko
// operatori su isto tip fuzzyseta
//
FuzzySet** Parser::createOperatorTree(node *root) {
	if(root == NULL) return NULL;
	else if(operation *opr = dynamic_cast<operation*>(root)) {
		FuzzySet **left = NULL, **right = NULL;
		if(opr->getLeft() != NULL) {
			left = createOperatorTree(opr->getLeft());
			delete opr->getLeft();
		}
		if(opr->getLeft() != NULL) {
			right = createOperatorTree(opr->getRight());
			delete opr->getRight();
		}
		if (left == NULL && right == NULL) throw 16;

		// tu sad treba provjeriti dal imaju iste domene
		if (left != NULL && right != NULL && opr->getValue() != MAMDANI)
			if(Info::Instance()->cmpDomain((*left)->getDomain(), (*right)->getDomain()) == false)
				throw 17;

		return Factory::operatorFactory(opr->getValue(), left, right);

	}
	else { // inace nije oprerator
		return Info::Instance()->getFuzzySetDoublePointer(root->getValue());
	}
}


void Parser::printTree(node *root, int d) {
	if( root == NULL ) return;
	else if (operation *opr = dynamic_cast<operation*>(root)) {
		for(int i = 0; i < d; i++) cout << " ";
		cout << opr->getValue() << endl;
		
		if(opr->getLeft() != NULL) {
			printTree(opr->getLeft(), d + 1);
		}
		if(opr->getRight() != NULL) {
			printTree(opr->getRight(), d + 1);
		}
	}

	else {
		for(int i = 0; i < d; i++) cout << " ";
		cout << root->getValue() << endl;
		return;
	}
}

void Parser::parseSymmetric(string input) {
	try {
		string key = removeSpaces(cutSubString(input, " ", ";"));
		FuzzySet *set = Info::Instance()->getFuzzySetPointer(key);

		cout << key << " je simetricna? ";

		bool value = Test::testSymmetric(set);

		cout << (value == false ? "NE" : "DA") << endl;
	}
	catch (int e) {
		cerr << "Greska " << "testSymmetric " << input << endl;
	}
}

void Parser::parseReflexive(string input) { 
	try {
		string key = removeSpaces(cutSubString(input, " ", ";"));
		FuzzySet *set = Info::Instance()->getFuzzySetPointer(key);

		cout << key << " je rekleksivna? ";

		bool value = Test::testReflexive(set);

		cout << (value == false ? "NE" : "DA") << endl;
	}
	catch (int e) {
		cerr << "Greska " << "testSymmetric " << input << endl;
	}
}
		
void Parser::parseTransitive(string input) {
	try {
		string f = splitString(input, ",")[ 0];
		string key = removeSpaces(f.substr(f.find(" ")));
		string type = removeSpaces(cutSubString(input, "'", ";"));
		type = type.substr(0, type.size()-1);
		FuzzySet *set = Info::Instance()->getFuzzySetPointer(key);

		cout << key << " je "<< type << " tranzitivna? ";
		bool value = Test::testTransitive(set, type);

		cout << (value == false ? "NE" : "DA") << endl;
	}
	catch (int e) {
		cerr << "Greska " << " testTransitive " << input << endl;
	}
}

void Parser::parseFuzzyEquivalence(string input) {
	try {
		string f = splitString(input, ",")[ 0];
		string key = removeSpaces(f.substr(f.find(" ")));
		string type = removeSpaces(cutSubString(input, "'", ";"));
		type = type.substr(0, type.size()-1);
		FuzzySet *set = Info::Instance()->getFuzzySetPointer(key);

		cout << key << " je "<< type << "-neizrazita relacije ekvivalencije? ";
		bool value = Test::testFuzzyEquivalence(set, type);

		cout << (value == false ? "NE" : "DA") << endl;
	}
	catch (int e) {
		cerr << "Greska " << " testFuzzyEquivalence " << input << endl;
	}
}

void Parser::parseProject(string input) {
	try{
		string key = begin2first_delimiter(input , ":");
		input = input.substr(input.find("project") + 7);
		string name = removeSpaces(input.substr(0, input.find("@") - 1));
		string dom = input.substr(input.find("@")+1);
		dom = removeSpaces(cutSubString(dom, " ",";"));

		Domain *d = Info::Instance()->getDomainPointer(dom);
		FuzzySet *f = Info::Instance()->getFuzzySetPointer(name);


		FuzzySet *project = Factory::createProject(f, d);
		Info::Instance()->registerFuzzySet(key, project);
	}
	catch (int e) {
		cerr << "Greska projekcija " << input << endl;
	}
}
		
void Parser::parseCylindrical_extension(string input) {
	try {
		string key = begin2first_delimiter(input , ":");
		input = input.substr(input.find("cylindrical_extension") + 22);
		string name = removeSpaces(input.substr(0, input.find("@") - 1));
		string dom = input.substr(input.find("@")+1);
		dom = removeSpaces(cutSubString(dom, " ",";"));

		Domain *d = Info::Instance()->getDomainPointer(dom);
		FuzzySet *f = Info::Instance()->getFuzzySetPointer(name);

		FuzzySet *extension = Factory::createCylindricalExtension(f, d);
		Info::Instance()->registerFuzzySet(key, extension);
	}
	catch (int e) {
		cerr << "Greska cilindricno prosirenje " << input << endl;
	}
}
		
void Parser::parseComposition(string input) {
	try {
		string key = begin2first_delimiter(input , ":");
		string type = removeSpaces(input.substr(input.find("using") + 5));
		type = type.substr(1, type.size() - 3);
		input = input.substr(input.find("composition") + 12);
		input = removeSpaces(input.substr(0, input.find("using")));

		vector< string > fs = splitString(input, ",");
		if (fs.size() != 2) throw 20;

		FuzzySet *first = Info::Instance()->getFuzzySetPointer(fs.at(0));
		FuzzySet *second = Info::Instance()->getFuzzySetPointer(fs.at(1));


		FuzzySet *composition = Factory::createComposition(first, second, type);
		Info::Instance()->registerFuzzySet(key, composition);
	}
	catch (int e) {
		cerr << "Greska kompozicije!" << endl;
	}
}
