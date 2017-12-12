#include "E-commercePlatform.h"


ifstream ifp;												//�����ļ���
ofstream ofp;												//����ļ���


vector<string> Product::categories;
vector<double> Product::discountOfC;
vector<string> Product::offerOfC;
int Product::amountOfC;					


void Product::readCategories() {
	double discount;
	string buffer;
	ifp.open("File/Category.txt", ios::in);
	if (!ifp) {
		cout << "Category.txt�޷��򿪣�" << endl;
		return;
	}
	while (!ifp.eof()) {
		getline(ifp, buffer);
		categories.push_back(buffer);
		ifp >> discount;
		discountOfC.push_back(discount);
		getline(ifp, buffer);
		getline(ifp, buffer);
		offerOfC.push_back(buffer);
	}
	ifp.close();
	amountOfC = categories.size();
}

void Product::readAnnotation() {
	int i;
	string buffer;
	ifp.open("File/Product.txt", ios::in);
	if (!ifp) {
		cout << "Product.txt�޷��򿪣�" << endl;
		return;
	}
	for (i = 1; i <= 15; i++)
		getline(ifp, buffer);
}

void Product::writeAnnotation() {
	string buffer;
	ifp.open("File/Product Annotation.txt", ios::in);
	if (!ifp) {
		cout << "Product Annotation.txt�޷��򿪣�" << endl;
		return;
	}
	ofp.open("File/Product.txt", ios::out);
	if (!ofp) {
		cout << "Product.txt�޷��򿪣�" << endl;
		return;
	}
	while (!ifp.eof()) {
		getline(ifp, buffer);
		ofp << buffer << endl;
	}
	ifp.close();
}

void Product::readProduct() {
	int sales;
	int surplus;
	double price;
	double fee;
	double discount;
	string buffer;
	getline(ifp, buffer);
	name = buffer;
	ifp >> price;
	originalPrice = price;
	getline(ifp, buffer);
	ifp >> price;
	currentPrice = price;
	getline(ifp, buffer);
	ifp >> fee;
	expressFee = fee;
	getline(ifp, buffer);
	ifp >> sales;
	this->sales = sales;
	getline(ifp, buffer);
	getline(ifp, buffer);
	storeName = buffer;
	getline(ifp, buffer);
	storeAddress = buffer;
	ifp >> surplus;
	productSurplus = surplus;
	getline(ifp, buffer);
	ifp >> discount;
	getline(ifp, buffer);
	getline(ifp, buffer);
	getline(ifp, buffer);
	intro = buffer;
}

void Product::readForUser() {
	int sales;
	int surplus;
	int amount;
	int state;
	double time;
	double price;
	double fee;
	double discount;
	string buffer;
	getline(ifp, buffer);
	name = buffer;
	ifp >> price;
	originalPrice = price;
	getline(ifp, buffer);
	ifp >> price;
	currentPrice = price;
	getline(ifp, buffer);
	ifp >> fee;
	expressFee = fee;
	getline(ifp, buffer);
	ifp >> sales;
	this->sales = sales;
	getline(ifp, buffer);
	getline(ifp, buffer);
	storeName = buffer;
	getline(ifp, buffer);
	storeAddress = buffer;
	ifp >> surplus;
	productSurplus = surplus;
	getline(ifp, buffer);
	ifp >> discount;
	getline(ifp, buffer);
	getline(ifp, buffer);
	ifp >> amount;
	purchaseQuantity = amount;
	getline(ifp, buffer);
	ifp >> state;
	this->state = state;
	getline(ifp, buffer);
	ifp >> time;
	payTime = time;
	getline(ifp, buffer);
	ifp >> time;
	deliveryTime = time;
	getline(ifp, buffer);
}

void Product::writeProduct() {
	ofp << getCategory() << endl;
	ofp << name << endl;
	ofp << originalPrice << endl;
	ofp << currentPrice << endl;
	ofp << expressFee << endl;
	ofp << sales << endl;
	ofp << storeName << endl;
	ofp << storeAddress << endl;
	ofp << productSurplus << endl;
	ofp << discount << endl;
	ofp << offer << endl;
	ofp << intro << endl;
}

void Product::writeForUser() {
	ofp << getCategory() << endl;
	ofp << name << endl;
	ofp << originalPrice << endl;
	ofp << currentPrice << endl;
	ofp << expressFee << endl;
	ofp << sales << endl;
	ofp << storeName << endl;
	ofp << storeAddress << endl;
	ofp << productSurplus << endl;
	ofp << discount << endl;
	ofp << offer << endl;
	ofp << purchaseQuantity << endl;
	ofp << state << endl;
	ofp << payTime << endl;
	ofp << deliveryTime << endl;
}

void Product::setCategory(string category) {
	this->category = category;
}

void Product::setPurchaseQuantity(int quantity) {
	purchaseQuantity = quantity;
}

void Product::setPurchaseQuantity(int quantity, string s) {
	purchaseQuantity += quantity;
}

void Product::setState(int state) {
	this->state = state;
}

void Product::setPayTime(double time) {
	payTime = time;
}

void Product::setDeliveryTime(double time) {
	deliveryTime = time;
}

void Product::setDiscount(double discount) {
	this->discount = discount;
}

void Product::setOffer(string buffer) {
	offer = buffer;
}

vector<string> Product::getCategories() {
	return categories;
}

vector<double> Product::getDiscountOfC() {
	return discountOfC;
}

vector<string> Product::getOfferOfC() {
	return offerOfC;
}

string Product::getName() {
	return name;
}

double Product::getCurrentPrice() {
	return currentPrice;
}

string Product::getCategory() {
	return category;
}

double Product::getExpressFee() {
	return expressFee;
}

int Product::getPurchaseQuantity() {
	return purchaseQuantity;
}

int Product::getAmountOfC() {
	return amountOfC;
}

int Product::getState() {
	return state;
}

double Product::getPayTime() {
	return payTime;
}

double Product::getDeliveryTime() {
	return deliveryTime;
}

double Product::getDiscount() {
	return discount;
}

string Product::getOffer() {
	return offer;
}

void Product::addSales(int amount) {
	sales += amount;
}

void Product::decProductS(int amount) {
	productSurplus -= amount;
}

void Product::priProduct() {
	cout << ">>* " << storeName << " *<<" << endl;
	cout << name << endl;
	printf("�ּۣ���%.2f\t\tԭ�ۣ���%.2f", currentPrice, originalPrice);
	cout << "\t\tX " << purchaseQuantity << endl;
	printf("�˷ѣ�%.2f\n", expressFee);
}

void Product::priProductInList() {
	vector<string> buffer;
	cout << name << endl;
	splitString(offer, buffer, " ");
	if (atoi(buffer[0].c_str()))
		cout << "��" << buffer[0] << "��" << buffer[1];
	else
		cout << "�������";
	if (discount)
		printf("\t\tƷ���ۿۣ�%.2f��\n", discount);
	else
		cout << "\t\t��Ʒ���ۿ�" << endl;
	printf("��%.2f", currentPrice);
	cout << "\t\t" << sales << "�˸���" << endl;
	cout << "��ҵ�ַ��" << storeAddress << endl << endl;
}

void Product::priDetailedProduct() {
	vector<string> buffer;
	cout << "******************************************************************************************" << endl;
	cout << name << endl;
	printf("��%.2f", currentPrice);
	cout << "\t\tƷ���һ�" << endl;
	printf("ԭ�ۣ�%.2f\n", originalPrice);
	cout << "�˷ѣ�";
	printf("%.2f", expressFee);
	cout << "\t\t������" << sales << " ��\t\t" << storeAddress << endl;
	splitString(offer, buffer, " ");
	if (atoi(buffer[0].c_str()))
		cout << "��" << buffer[0] << "��" << buffer[1];
	else
		cout << "�������";
	if (discount)
		printf("\t\tƷ���ۿۣ�%.2f��\n\n", discount);
	else
		cout << "\t\t��Ʒ���ۿ�" << endl << endl;
	cout << "��Ʒ������" << intro << endl;
	cout << ">>* " << storeName << " *<<" << endl;
	cout << "******************************************************************************************" << endl;
}

void Product::priInfo() {
	cout << "******************************************************************************************" << endl;
	cout << name << endl;
	cout << "��� " << productSurplus << " ��" << endl;
}

Product::Product() {
	expressFee = 0;
	sales = 0;
	state = 0;
	payTime = 0;
	deliveryTime = 0;
	purchaseQuantity = 0;
}

Product::~Product() {

}

void  Cosmetic::setCategory(string name) {
	category = name;
}

string Cosmetic::getCategory() {
	return category;
}

Cosmetic::Cosmetic() {
	setCategory("��ױ");
}

Cosmetic::~Cosmetic() {

}

void  SportsOutdoor::setCategory(string name) {
	category = name;
}

string SportsOutdoor::getCategory() {
	return category;
}

SportsOutdoor::SportsOutdoor() {
	setCategory("�˶�����");
}

SportsOutdoor::~SportsOutdoor() {

}

void  ArticlesOfDailyUse::setCategory(string name) {
	category = name;
}

string ArticlesOfDailyUse::getCategory() {
	return category;
}

ArticlesOfDailyUse::ArticlesOfDailyUse() {
	setCategory("�ٻ�");
}

ArticlesOfDailyUse::~ArticlesOfDailyUse() {

}

void  Women::setCategory(string name) {
	category = name;
}

string Women::getCategory() {
	return category;
}

Women::Women() {
	setCategory("Ůװ");
}

Women::~Women() {

}

void  Men::setCategory(string name) {
	category = name;
}

string Men::getCategory() {
	return category;
}

Men::Men() {
	setCategory("��װ");
}

Men::~Men() {

}

void  Food::setCategory(string name) {
	category = name;
}

string Food::getCategory() {
	return category;
}

Food::Food() {
	setCategory("ʳƷ");
}

Food::~Food() {

}