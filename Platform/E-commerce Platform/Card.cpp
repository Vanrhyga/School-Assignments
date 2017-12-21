#include "E-commercePlatform.h"


vector<string> Card::banks;			
extern ifstream ifp;
extern ofstream ofp;


void Card::readBank() {
	string buffer;
	ifp.open("File/Bank.txt", ios::in);
	if (!ifp) {
		cout << "Bank.txt无法打开！" << endl;
		return;
	}
	while (!ifp.eof()) {
		getline(ifp, buffer);
		banks.push_back(buffer);
	}
	ifp.close();
}

void Card::setCardID(char* cardID) {
	strcpy_s(this->cardID, cardID);
}

void Card::setBank(string bank) {
	this->bank = bank;
}

void Card::setPassword(string password) {
	this->password = password;
}

string Card::getBank() {
	return bank;
}

char* Card::getCardID() {
	return cardID;
}

string Card::getPassword() {
	return password;
}

void Card::readCard() {
	string buffer;
	ifp >> cardID;
	getline(ifp, buffer);
	getline(ifp, buffer);
	bank = buffer;
	getline(ifp, buffer);
	password = buffer;
}

void Card::writeCard() {
	ofp << cardID << endl;
	ofp << bank << endl;
	ofp << password << endl;
}

vector<string> Card::getBanks() {
	return banks;
}

Card::Card() {

}

Card::~Card() {

}