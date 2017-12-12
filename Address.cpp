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
			cout << "手机号有误！请输入正确信息：" << endl;
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
	cout << "收货人：" << consignee << endl;
	cout << "联系电话：" << contactNumber << endl;
	cout << "具体地址：" << detailedAddress << endl;
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

