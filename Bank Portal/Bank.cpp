#include "bankPortal.h"


extern ifstream ifp;


void Bank::read() {
	getline(ifp, name);
}

string Bank::getName() {
	return name;
}

Bank::Bank(){
}

Bank::~Bank(){
}
