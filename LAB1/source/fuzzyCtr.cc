
/*
		Program koji koristi simulator
*/

#include <iostream>
#include <string>
#include <cstdio>
#include <fstream>

using namespace std; 

#include "control.h"

int main(void)
{	
	Rules *pravilaKormilo = new Rules();
	pravilaKormilo->initKormilo();

	Rules *pravilaAkcel = new Rules();
	pravilaAkcel->initAkceleracija();


	Decoder *decoder_ = new CenterOfArea;

	FuzzyControl *controlerKormilo = new MinimumEngine(pravilaKormilo, decoder_);
	FuzzyControl *controlerAkcel = new MinimumEngine(pravilaAkcel, decoder_);
	
		
	char buff[1000];
	int L, D, LK, DK, V, S, kormilo, akcel;


	while(true){
	  fgets(buff, 1000, stdin);
	  if(buff[ 0 ] == 'K') break;

	  sscanf(buff,"%d %d %d %d %d %d",&L, &D, &LK, &DK, &V, &S);

	  // fuzzy magic ...

	  vector< Object > v = { Object(int(L)), Object(int(D)), Object(int(LK)), 
	  											 Object(int(DK)), Object(int(V)), Object(int(S)) };

	  kormilo = (int) controlerKormilo->getOutput(v);

	  akcel = (int) controlerAkcel->getOutput(v);

	  fprintf(stdout,"%d %d\n",akcel,kormilo);
	  fflush(stdout);
	}


	// pobrisemo sve
	delete pravilaKormilo; delete decoder_;
	delete controlerKormilo ;delete controlerAkcel;
	delete pravilaAkcel;

	return 0;
}
