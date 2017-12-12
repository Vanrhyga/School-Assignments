#include "E-commercePlatform.h"


extern ifstream ifp;
extern ofstream ofp;


void Address::readAddress() {
	string buffer;
	getline(ifp, buffer);
	consignee = buffer;
	getline(ifp, buffer);
	contactNumber = buffer;
	getline(ifp, buffer);
	detailedAddress = buffer;
}

void Address::setConsignee(string consignee) {
	this->consignee = consignee;
}

void Address::setContactNumber(string contactNumber) {
	while (1) {
		if (contactNumber.length() == 11) {
			this->contactNumber = contactNumber;
			break;
		}
		else {
			cout << "�ֻ���������������ȷ��Ϣ��" << endl;
			cin >> contactNumber;
		}
	}
}

void Address::setDetailedAddress(string detailedAddress) {
	this->detailedAddress = detailedAddress;
}

string Address::getContactNumber() {
	return contactNumber;
}

void Address::priAddress() {
	cout << "�ջ��ˣ�" << consignee << endl;
	cout << "��ϵ�绰��" << contactNumber << endl;
	cout << "�����ַ��" << detailedAddress << endl;
}

void Address::writeAddress() {
	ofp << consignee << endl;
	ofp << contactNumber << endl;
	ofp << detailedAddress << endl;
}

Address::Address() {

}

Address::~Address() {

}

