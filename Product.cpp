#include "E-commercePlatform.h"


ifstream ifp;												//输入文件流
ofstream ofp;												//输出文件流


vector<string> Product::categories;
vector<double> Product::discountOfC;
vector<string> Product::offerOfC;
int Product::amountOfC;					


void Product::readCategories() {
	double discount;
	string buffer;
	ifp.open("File/Category.txt", ios::in);
	if (!ifp) {
		cout << "Category.txt无法打开！" << endl;
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
		cout << "Product.txt无法打开！" << endl;
		return;
	}
	for (i = 1; i <= 15; i++)
		getline(ifp, buffer);
}

void Product::writeAnnotation() {
	string buffer;
	ifp.open("File/Product Annotation.txt", ios::in);
	if (!ifp) {
		cout << "Product Annotation.txt无法打开！" << endl;
		return;
	}
	ofp.open("File/Product.txt", ios::out);
	if (!ofp) {
		cout << "Product.txt无法打开！" << endl;
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
	printf("现价：￥%.2f\t\t原价：￥%.2f", currentPrice, originalPrice);
	cout << "\t\tX " << purchaseQuantity << endl;
	printf("运费：%.2f\n", expressFee);
}

void Product::priProductInList() {
	vector<string> buffer;
	cout << name << endl;
	splitString(offer, buffer, " ");
	if (atoi(buffer[0].c_str()))
		cout << "满" << buffer[0] << "减" << buffer[1];
	else
		cout << "无满减活动";
	if (discount)
		printf("\t\t品类折扣：%.2f折\n", discount);
	else
		cout << "\t\t无品类折扣" << endl;
	printf("￥%.2f", currentPrice);
	cout << "\t\t" << sales << "人付款" << endl;
	cout << "店家地址：" << storeAddress << endl << endl;
}

void Product::priDetailedProduct() {
	vector<string> buffer;
	cout << "******************************************************************************************" << endl;
	cout << name << endl;
	printf("￥%.2f", currentPrice);
	cout << "\t\t品牌钜惠" << endl;
	printf("原价：%.2f\n", originalPrice);
	cout << "运费：";
	printf("%.2f", expressFee);
	cout << "\t\t销量：" << sales << " 笔\t\t" << storeAddress << endl;
	splitString(offer, buffer, " ");
	if (atoi(buffer[0].c_str()))
		cout << "满" << buffer[0] << "减" << buffer[1];
	else
		cout << "无满减活动";
	if (discount)
		printf("\t\t品类折扣：%.2f折\n\n", discount);
	else
		cout << "\t\t无品类折扣" << endl << endl;
	cout << "产品描述：" << intro << endl;
	cout << ">>* " << storeName << " *<<" << endl;
	cout << "******************************************************************************************" << endl;
}

void Product::priInfo() {
	cout << "******************************************************************************************" << endl;
	cout << name << endl;
	cout << "库存 " << productSurplus << " 件" << endl;
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
	setCategory("美妆");
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
	setCategory("运动户外");
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
	setCategory("百货");
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
	setCategory("女装");
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
	setCategory("男装");
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
	setCategory("食品");
}

Food::~Food() {

}