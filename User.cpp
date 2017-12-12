#include "E-commercePlatform.h"


extern vector<Cosmetic> cosmeticList;
extern vector<SportsOutdoor> sportsList;
extern vector<ArticlesOfDailyUse> articlesOfDUList;
extern vector<Women> womenList;
extern vector<Men> menList;
extern vector<Food> foodList;
vector<Cosmetic> cosmeticForSC;
vector<SportsOutdoor> sportsForSC;
vector<ArticlesOfDailyUse> articlesOfDUForSC;
vector<Women> womenForSC;
vector<Men> menForSC;
vector<Food> foodForSC;
vector<Cosmetic> cosmeticForWFD;
vector<SportsOutdoor> sportsForWFD;
vector<ArticlesOfDailyUse> articlesOfDUForWFD;
vector<Women> womenForWFD;
vector<Men> menForWFD;
vector<Food> foodForWFD;
vector<Cosmetic> cosmeticForWFS;
vector<SportsOutdoor> sportsForWFS;
vector<ArticlesOfDailyUse> articlesOfDUForWFS;
vector<Women> womenForWFS;
vector<Men> menForWFS;
vector<Food> foodForWFS;
extern double systemTime;
extern ifstream ifp;
extern ofstream ofp;


void User::readAnnotation() {
	int i;
	string buffer;
	ifp.open("File/User.txt", ios::in);
	if (!ifp) {
		cout << "User.txt无法打开！" << endl;
		return;
	}
	for (i = 1; i <= 36; i++)
		getline(ifp, buffer);
}

void User::writeAnnotation() {
	string buffer;
	ifp.open("File/User Annotation.txt", ios::in);
	if (!ifp) {
		cout << "User Annotation.txt无法打开！" << endl;
		return;
	}
	ofp.open("File/User.txt", ios::out);
	if (!ofp) {
		cout << "User.txt无法打开！" << endl;
		return;
	}
	while (!ifp.eof()) {
		getline(ifp, buffer);
		ofp << buffer << endl;
	}
	ifp.close();
}

void User::read() {
	int i;
	unsigned int j;
	int amount;
	string buffer;
	Cosmetic cosmetic;
	SportsOutdoor sports;
	ArticlesOfDailyUse articlesOfDU;
	Women women;
	Men men;
	Food food;
	for (i = 0; i < Product::getAmountOfC(); i++)
		if (Product::getCategories()[i] == "美妆")
			break;
	cosmetic.setDiscount(Product::getDiscountOfC()[i]);
	cosmetic.setOffer(Product::getOfferOfC()[i]);
	for (i = 0; i < Product::getAmountOfC(); i++)
		if (Product::getCategories()[i] == "运动户外")
			break;
	sports.setDiscount(Product::getDiscountOfC()[i]);
	sports.setOffer(Product::getOfferOfC()[i]);
	for (i = 0; i < Product::getAmountOfC(); i++)
		if (Product::getCategories()[i] == "百货")
			break;
	articlesOfDU.setDiscount(Product::getDiscountOfC()[i]);
	articlesOfDU.setOffer(Product::getOfferOfC()[i]);
	for (i = 0; i < Product::getAmountOfC(); i++)
		if (Product::getCategories()[i] == "女装")
			break;
	women.setDiscount(Product::getDiscountOfC()[i]);
	women.setOffer(Product::getOfferOfC()[i]);
	for (i = 0; i < Product::getAmountOfC(); i++)
		if (Product::getCategories()[i] == "男装")
			break;
	men.setDiscount(Product::getDiscountOfC()[i]);
	men.setOffer(Product::getOfferOfC()[i]);
	for (i = 0; i < Product::getAmountOfC(); i++)
		if (Product::getCategories()[i] == "食品")
			break;
	food.setDiscount(Product::getDiscountOfC()[i]);
	food.setOffer(Product::getOfferOfC()[i]);
	for (i = 0; i < amountOfSC; i++) {
		getline(ifp, buffer);
		if (buffer == "美妆") {
			amount = cosmeticForSC.size();
			cosmetic.readForUser();
			cosmeticForSC.push_back(cosmetic);
		}
		else if (buffer == "运动户外") {
			amount = sportsForSC.size();
			sports.readForUser();
			sportsForSC.push_back(sports);
		}
		else if (buffer == "百货") {
			amount = articlesOfDUForSC.size();
			articlesOfDU.readForUser();
			articlesOfDUForSC.push_back(articlesOfDU);
		}
		else if (buffer == "女装") {
			amount = womenForSC.size();
			women.readForUser();
			womenForSC.push_back(women);
		}
		else if (buffer == "男装") {
			amount = menForSC.size();
			menForSC.push_back(men);
			menForSC.push_back(men);
		}
		else if (buffer == "食品") {
			amount = foodForSC.size();
			food.readForUser();
			foodForSC.push_back(food);
		}
	}
	for (j = 0; j < cosmeticForSC.size(); j++)
		shoppingCart.push_back(&cosmeticForSC[j]);
	for (j = 0; j < sportsForSC.size(); j++)
		shoppingCart.push_back(&sportsForSC[j]);
	for (j = 0; j < articlesOfDUForSC.size(); j++)
		shoppingCart.push_back(&articlesOfDUForSC[j]);
	for (j = 0; j < womenForSC.size(); j++)
		shoppingCart.push_back(&womenForSC[j]);
	for (j = 0; j < menForSC.size(); j++)
		shoppingCart.push_back(&menForSC[j]);
	for (j = 0; j < foodForSC.size(); j++)
		shoppingCart.push_back(&foodForSC[j]);
	ifp >> amount;
	amountOfWFD = amount;
	getline(ifp, buffer);
	for (i = 0; i < amountOfWFD; i++) {
		getline(ifp, buffer);
		if (buffer == "美妆") {
			amount = cosmeticForWFD.size();
			cosmetic.readForUser();
			cosmeticForWFD.push_back(cosmetic);
		}
		else if (buffer == "运动户外") {
			amount = sportsForWFD.size();
			sports.readForUser();
			sportsForWFD.push_back(sports);
		}
		else if (buffer == "百货") {
			amount = articlesOfDUForWFD.size();
			articlesOfDU.readForUser();
			articlesOfDUForWFD.push_back(articlesOfDU);
		}
		else if (buffer == "女装") {
			amount = womenForWFD.size();
			women.readForUser();
			womenForWFD.push_back(women);
		}
		else if (buffer == "男装") {
			amount = menForWFD.size();
			men.readForUser();
			menForWFD.push_back(men);
		}
		else if (buffer == "食品") {
			amount = foodForWFD.size();
			food.readForUser();
			foodForWFD.push_back(food);
		}
	}
	for (j = 0; j < cosmeticForWFD.size(); j++)
		waitForDeliver.push_back(&cosmeticForWFD[j]);
	for (j = 0; j < sportsForWFD.size(); j++)
		waitForDeliver.push_back(&sportsForWFD[j]);
	for (j = 0; j < articlesOfDUForWFD.size(); j++)
		waitForDeliver.push_back(&articlesOfDUForWFD[j]);
	for (j = 0; j < womenForWFD.size(); j++)
		waitForDeliver.push_back(&womenForWFD[j]);
	for (j = 0; j < menForWFD.size(); j++)
		waitForDeliver.push_back(&menForWFD[j]);
	for (j = 0; j < foodForWFD.size(); j++)
		waitForDeliver.push_back(&foodForWFD[j]);
	ifp >> amount;
	amountOfWFS = amount;
	getline(ifp, buffer);
	for (i = 0; i < amountOfWFS; i++) {
		getline(ifp, buffer);
		if (buffer == "美妆") {
			amount = cosmeticForWFS.size();
			cosmetic.readForUser();
			cosmeticForWFS.push_back(cosmetic);
		}
		else if (buffer == "运动户外") {
			amount = sportsForWFS.size();
			sports.readForUser();
			sportsForWFS.push_back(sports);
		}
		else if (buffer == "百货") {
			amount = articlesOfDUForWFS.size();
			articlesOfDU.readForUser();
			articlesOfDUForWFS.push_back(articlesOfDU);
		}
		else if (buffer == "女装") {
			amount = womenForWFS.size();
			women.readForUser();
			womenForWFS.push_back(women);
		}
		else if (buffer == "男装") {
			amount = menForWFS.size();
			men.readForUser();
			menForWFS.push_back(men);
		}
		else if (buffer == "食品") {
			amount = foodForWFS.size();
			food.readForUser();
			foodForWFS.push_back(food);
		}
	}
	for (j = 0; j < cosmeticForWFS.size(); j++)
		waitForSignature.push_back(&cosmeticForWFS[j]);
	for (j = 0; j < sportsForWFS.size(); j++)
		waitForSignature.push_back(&sportsForWFS[j]);
	for (j = 0; j < articlesOfDUForWFS.size(); j++)
		waitForSignature.push_back(&articlesOfDUForWFS[j]);
	for (j = 0; j < womenForWFS.size(); j++)
		waitForSignature.push_back(&womenForWFS[j]);
	for (j = 0; j < menForWFS.size(); j++)
		waitForSignature.push_back(&menForWFS[j]);
	for (j = 0; j < foodForWFS.size(); j++)
		waitForSignature.push_back(&foodForWFS[j]);
}

void User::readAddress() {
	int i;
	Address address;
	for (i = 0; i < amountOfSA; i++) {
		address.readAddress();
		shippingAddress.push_back(address);
	}
	if (amountOfSA) {
		address.readAddress();
		defaultAddress = address;
	}
}

void User::writeUser() {
	unsigned int i;
	ofp << username << endl;
	ofp << amountOfSA << endl;
	for (i = 0; i < shippingAddress.size(); i++)
		shippingAddress[i].writeAddress();
	if(amountOfSA)
		defaultAddress.writeAddress();
	ofp << phoneNumber << endl;
	ofp << password << endl;
	ofp << amountOfSC << endl;
	for (i = 0; i < shoppingCart.size(); i++)
		shoppingCart[i]->writeForUser();
	ofp << amountOfWFD << endl;
	for (i = 0; i < waitForDeliver.size(); i++) 
		waitForDeliver[i]->writeForUser();
	ofp << amountOfWFS << endl;
	for (i = 0; i < waitForSignature.size(); i++) 
		waitForSignature[i]->writeForUser();
}

void User::setUsername(string name) {
	username = name;
}

string User::getUsername() {
	return username;
}

void User::setAmountOfSA(int amount) {
	amountOfSA = amount;
}

vector<Address> User::getShippingAddress() {
	return shippingAddress;
}

void User::addShippingAddress() {
	string name, phoneNumber, detailedAddress;
	Address address;
	cout << endl<<"请输入收货人姓名：" << endl;
	cin >> name;
	address.setConsignee(name);
	cout << "请输入联系电话：" << endl;
	cin >> phoneNumber;
	address.setContactNumber(phoneNumber);
	cout << "请输入具体地址：" << endl;
	cin >> detailedAddress;
	address.setDetailedAddress(detailedAddress);
	shippingAddress.push_back(address);
	amountOfSA++;
	if (amountOfSA == 1)
		defaultAddress = shippingAddress[0];
	cout << "..." << endl;
	Sleep(3000);
	cout << "增添成功！" << endl;
	cout << "*****************************************************************************************" << endl;
}

void User::modifyShippingAddress() {
	unsigned int i, j, times=0;
	int change=0;
	string buffer, name, phoneNumber, detailedAddress, choice;
	vector<string> choices;
	vector<string> changes;
	cout << endl << "请输入需修改的收货地址序号，以空格为分界符：" << endl;
	getline(cin, choice);
	getline(cin, choice);
	splitString(choice, choices, " ");
	for (i = 0; i < choices.size(); i++) {
		if (defaultAddress.getContactNumber() == shippingAddress[atoi(choices[i].c_str()) - 1].getContactNumber()) {
			change = 1;
			times++;
		}
		cout << "*****************************************************************************************" << endl;
		shippingAddress[atoi(choices[i].c_str())-1].priAddress();
		cout << endl<<"可修改的内容如下：" << endl;
		cout << "1.收货人\t\t\t2.联系电话\t\t\t3.具体地址" << endl << endl;
		cout << "请输入需修改的内容序号：" << endl;
		if (i)
			getline(cin, buffer);
		getline(cin, buffer);
		changes.clear();
		splitString(buffer, changes, " ");
		for (j = 0; j < changes.size(); j++) {
			if (atoi(changes[j].c_str()) == 1) {
				cout << endl << "请输入收货人姓名：" << endl;
				cin >> name;
				shippingAddress[atoi(choices[i].c_str()) - 1].setConsignee(name);
			}
			else if (atoi(changes[j].c_str()) == 2) {
				cout << endl << "请输入联系电话：" << endl;
				cin >> phoneNumber;
				shippingAddress[atoi(choices[i].c_str()) - 1].setContactNumber(phoneNumber);
			}
			else if (atoi(changes[j].c_str()) == 3) {
				cout << endl << "请输入具体地址：" << endl;
				cin >> detailedAddress;
				shippingAddress[atoi(choices[i].c_str()) - 1].setDetailedAddress(detailedAddress);
			}
		}
		cout << "..." << endl;
		Sleep(3000);
		cout << "修改成功！" << endl;
		if (change&&times==1)
			defaultAddress = shippingAddress[atoi(choices[i].c_str()) - 1];
	}
}

void User::delShippingAddress() {
	unsigned int i, j;
	int changes = 0;
	string choice;
	vector<string> choices;
	vector<Address>::iterator iter;
	cout << endl<<"请依次输入需删除的收货地址序号，以空格为分界符：" << endl;
	getline(cin, choice);
	getline(cin, choice);
	splitString(choice, choices, " ");
	for (i = 0; i < choices.size(); i++) {
		if (defaultAddress.getContactNumber() == shippingAddress[atoi(choices[i].c_str()) - 1 - i].getContactNumber())
			changes = 1;
		iter = shippingAddress.begin();
		for (j = 0; j < atoi(choices[i].c_str()) - 1 - i; j++)
			iter++;
		shippingAddress.erase(iter);
	}
	cout << "..." << endl;
	Sleep(3000);
	cout << "删除成功！" << endl;
	cout << "*****************************************************************************************" << endl;
	amountOfSA -= choices.size();
	if (changes&&amountOfSA)
		defaultAddress = shippingAddress[0];
}

void User::priShippingAddress() {
	unsigned int i;
	cout << ">>*                                   收货地址                                   *<<" << endl;
	for (i = 0; i < shippingAddress.size(); i++) {
		if (i)
			cout << endl;
		cout <<i + 1 << ".";
		shippingAddress[i].priAddress();
	}
	if (shippingAddress.size()) {
		cout << endl << ">>*                                   默认地址                                   *<<" << endl;
		defaultAddress.priAddress();
	}
}

Address User::getDA() {
	return defaultAddress;
}

void User::modifyDefaultAddress() {
	int choice;
	cout << endl<<"请输入默认收货地址序号：" << endl;
	cin >> choice;
	defaultAddress = shippingAddress[choice - 1];
	cout << "..." << endl;
	Sleep(3000);
	cout << "修改成功！" << endl;
	cout << "*****************************************************************************************" << endl;
}

void User::setPhoneNumber(string number) {
	while (1) {
		if (number.length() == 11) {
			this->phoneNumber = number;
			break;
		}
		else {
			cout << "手机号有误！请输入正确信息：" << endl;
			cin >> number;
		}
	}
}

string User::getPhoneNumber() {
	return phoneNumber;
}

void User::setPassword(string password) {
	this->password = password;
}

string User::getPassword() {
	return password;
}

void User::modifyPassword() {
	string password,confirmPassword;
	cout << endl << "请输入密码：" << endl;
	while (1) {
		while (1) {
			cin >> password;
			if (password.length() < 6)
				cout << "密码长度至少为6位！请您重新输入：" << endl;
			else
				break;
		}
		cout << "请再次确认密码：" << endl;
		cin >> confirmPassword;
		if (confirmPassword == password)
			break;
		else
			cout << "两次密码不相同！请重新输入您的密码：" << endl;
	}
	this->password = password;
	cout << "..." << endl;
	Sleep(3000);
	cout << "修改成功！" << endl;
}

void User::modifyName() {
	string name;
	cout << endl << "请输入会员名：" << endl;
	cin >> name;
	username = name;
	cout << "..." << endl;
	Sleep(3000);
	cout << "修改成功！" << endl;
}

void User::setAmountOfSC(int amount) {
	amountOfSC = amount;
}

vector<Product*> User::getSC() {
	return shoppingCart;
}

int User::getAmountOfSC() {
	return amountOfSC;
}

void User::addShoppingCart(Product* product) {
	unsigned int i;
	for (i = 0; i < shoppingCart.size(); i++)
		if (shoppingCart[i]->getName() == product->getName()) 
			return;
	shoppingCart.push_back(product);
	amountOfSC++;
}

int User::priShoppingCart() {
	unsigned int i;
	vector<string> buffer;
	if (shoppingCart.size()) {
		cout << ">>*                                       购物车                                       *<<" << endl;
		cout << "******************************************************************************************" << endl;
		for (i = 0; i < shoppingCart.size(); i++) {
			cout << i + 1 << ".";
			shoppingCart[i]->priProduct();
			buffer.clear();
			splitString(shoppingCart[i]->getOffer(), buffer, " ");
			if (atoi(buffer[0].c_str())!=0) 
				cout << "优惠活动：（同类商品）满" << buffer[0] << "减" << buffer[1];
			else
				cout << "无满减优惠活动";
			if (shoppingCart[i]->getDiscount())
				printf("\t\t品类折扣：%.2f折\n\n", shoppingCart[i]->getDiscount());
			else
				cout << "\t\t无品类折扣\n\n" << endl;
		}
		return 1;
	}
	else {
		cout << "您的购物车空空如也，快去选购吧！(づ￣ 3￣)づ" << endl;
		return 0;
	}
}

void User::emptyShoppingCart() {
	unsigned int i, amount = shoppingCart.size();
	for (i = 0; i < amount; i++) 
		shoppingCart.erase(shoppingCart.begin());
	cout << "..." << endl;
	Sleep(3000);
	cout << "清空成功！" << endl;
	amountOfSC = 0;
}

void User::delPartOfSC() {
	unsigned int i,j;
	string choice;
	vector<string> choices;
	vector<Product*>::iterator iter;
	cout << endl << "请依次输入需删除的商品序号，以空格为分界符：" << endl;
	getline(cin, choice);
	getline(cin, choice);
	splitString(choice, choices, " ");
	for (i = 0; i < choices.size(); i++) {
		iter = shoppingCart.begin();
		for (j = 0; j < atoi(choices[i].c_str()) - 1 - i; j++)
			iter++;
		shoppingCart.erase(iter);
	}
	cout << "..." << endl;
	Sleep(3000);
	cout << "删除成功！" << endl;
	amountOfSC -= choices.size();
}

void User::modifyPurchaseQuantity() {
	int choice, amount;
	cout << endl << "请选择需修改的产品序号：" << endl;
	cin >> choice;
	cout << "请输入购买数量：" << endl;
	cin >> amount;
	shoppingCart[choice - 1]->setPurchaseQuantity(amount);
	cout << "..." << endl;
	Sleep(3000);
	cout<< "修改成功！" << endl;
}

double User::clearPartOfSC() {
	unsigned int i, j;
	int k = 0, a;
	int partOfC[MAXCATEGORIES];
	double sum = 0;
	double partOfSum[MAXCATEGORIES];
	string choice;
	vector<string> choices;
	vector<string> buffer;
	vector<Product*>::iterator iter;
	for (k = 0; k < MAXCATEGORIES; k++) {
		partOfC[k] = -1;
		partOfSum[k] = 0;
	}
	cout << endl << "请输入需结算的产品序号，以空格为分界符：" << endl;
	getline(cin, choice);
	getline(cin, choice);
	splitString(choice, choices, " ");
	for (k=0,a = 0; a < Product::getAmountOfC(); a++) {
		for (j = 0; j < choices.size(); j++) {
			if (shoppingCart[atoi(choices[j].c_str()) - 1]->getCategory() == Product::getCategories()[a]) {
				partOfSum[k] += (shoppingCart[atoi(choices[j].c_str()) - 1]->getCurrentPrice() + shoppingCart[atoi(choices[j].c_str()) - 1]->getExpressFee())*shoppingCart[atoi(choices[j].c_str()) - 1]->getPurchaseQuantity();
				partOfC[k] = a;
			}
		}
		if (partOfSum[k])
			k++;
	}
	for (i = 0; i < choices.size(); i++) {
		for (j = 0; j < cosmeticList.size(); j++) 
			if (shoppingCart[atoi(choices[i].c_str()) - 1]->getName() == cosmeticList[j].getName()) {
				cosmeticList[j].addSales(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				cosmeticList[j].decProductS(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				cosmeticList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < sportsList.size(); j++)
			if (shoppingCart[atoi(choices[i].c_str()) - 1]->getName() == sportsList[j].getName()) {
				sportsList[j].addSales(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				sportsList[j].decProductS(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				sportsList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < articlesOfDUList.size(); j++)
			if (shoppingCart[atoi(choices[i].c_str()) - 1]->getName() == articlesOfDUList[j].getName()) {
				articlesOfDUList[j].addSales(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				articlesOfDUList[j].decProductS(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				articlesOfDUList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < womenList.size(); j++)
			if (shoppingCart[atoi(choices[i].c_str()) - 1]->getName() == womenList[j].getName()) {
				womenList[j].addSales(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				womenList[j].decProductS(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				womenList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < menList.size(); j++)
			if (shoppingCart[atoi(choices[i].c_str()) - 1]->getName() == menList[j].getName()) {
				menList[j].addSales(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				menList[j].decProductS(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				menList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < foodList.size(); j++)
			if (shoppingCart[atoi(choices[i].c_str()) - 1]->getName() == foodList[j].getName()) {
				foodList[j].addSales(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				foodList[j].decProductS(shoppingCart[atoi(choices[i].c_str()) - 1]->getPurchaseQuantity());
				foodList[j].setPurchaseQuantity(0);
			}
	}
	for (i = 0; i < choices.size(); i++) {
		shoppingCart[atoi(choices[i].c_str()) - 1 - i]->setPayTime(systemTime);
		waitForDeliver.push_back(shoppingCart[atoi(choices[i].c_str()) - 1 - i]);
		amountOfWFD++;
		iter = shoppingCart.begin();
		for (j = 0; j < atoi(choices[i].c_str()) - 1 - i; j++)
			iter++;
		shoppingCart.erase(iter);
	}
	cout << "*****************************************************************************************" << endl;
	for (i = 0; partOfC[i] != -1 && i<MAXCATEGORIES; i++) {
		switch (partOfC[i]) {
		case 0:
			buffer.clear();
			splitString(cosmeticList[0].getOffer(), buffer, " ");
			printf("美妆区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠金额为：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动";
			cout << endl;
			if (cosmeticList[0].getDiscount()) {
				partOfSum[i] *= cosmeticList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", cosmeticList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 1:
			buffer.clear();
			splitString(sportsList[0].getOffer(), buffer, " ");
			printf("运动户外区总金额（含邮费）：%.2f\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (sportsList[0].getDiscount()) {
				partOfSum[i] *= sportsList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", sportsList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 2:
			buffer.clear();
			splitString(articlesOfDUList[0].getOffer(), buffer, " ");
			printf("百货区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (articlesOfDUList[0].getDiscount()) {
				partOfSum[i] *= articlesOfDUList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", articlesOfDUList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 3:
			buffer.clear();
			splitString(womenList[0].getOffer(), buffer, " ");
			printf("女装区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (womenList[0].getDiscount()) {
				partOfSum[i] *= womenList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", womenList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 4:
			buffer.clear();
			splitString(menList[0].getOffer(), buffer, " ");
			printf("男装区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (menList[0].getDiscount()) {
				partOfSum[i] *= menList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", menList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 5:
			buffer.clear();
			splitString(foodList[0].getOffer(), buffer, " ");
			printf("食品区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (foodList[0].getDiscount()) {
				partOfSum[i] *= foodList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", foodList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
		}
	}
	for (i = 0; partOfSum[i] && i<MAXCATEGORIES; i++)
		sum += partOfSum[i];
	printf("\n\t\t\t\t\t\t\t\t合计（含邮费）：%.2f\n元", sum);
	cout << "..." << endl;
	Sleep(3000);
	cout << "支付成功！" << endl;
	amountOfSC -= choices.size();
	return sum;
}

double User::clearALLShoppingCart() {
	unsigned int i, j;
	int k = 0, a;
	double sum = 0;
	int partOfC[MAXCATEGORIES];
	double partOfSum[MAXCATEGORIES];
	vector<string> buffer;
	for (k = 0; k < MAXCATEGORIES; k++) {
		partOfC[k] = -1;
		partOfSum[k] = 0;
	}
	for (k=0,a = 0; a < Product::getAmountOfC(); a++) {
		for (j = 0; j < shoppingCart.size(); j++) {
			if (shoppingCart[j]->getCategory() == Product::getCategories()[a]) {
				partOfSum[k] += (shoppingCart[j]->getCurrentPrice() + shoppingCart[j]->getExpressFee())*shoppingCart[j]->getPurchaseQuantity();
				partOfC[k] = a;
			}
		}
		if (partOfSum[k])
			k++;
	}
	for (i = 0; i < shoppingCart.size(); i++) {
		for (j = 0; j < cosmeticList.size(); j++)
			if (shoppingCart[i]->getName() == cosmeticList[j].getName()) {
				cosmeticList[j].addSales(shoppingCart[i]->getPurchaseQuantity());
				cosmeticList[j].decProductS(shoppingCart[i]->getPurchaseQuantity());
				cosmeticList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < sportsList.size(); j++)
			if (shoppingCart[i]->getName() == sportsList[j].getName()) {
				sportsList[j].addSales(shoppingCart[i]->getPurchaseQuantity());
				sportsList[j].decProductS(shoppingCart[i]->getPurchaseQuantity());
				sportsList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < articlesOfDUList.size(); j++)
			if (shoppingCart[i]->getName() == articlesOfDUList[j].getName()) {
				articlesOfDUList[j].addSales(shoppingCart[i]->getPurchaseQuantity());
				articlesOfDUList[j].decProductS(shoppingCart[i]->getPurchaseQuantity());
				articlesOfDUList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < womenList.size(); j++)
			if (shoppingCart[i]->getName() == womenList[j].getName()) {
				womenList[j].addSales(shoppingCart[i]->getPurchaseQuantity());
				womenList[j].decProductS(shoppingCart[i]->getPurchaseQuantity());
				womenList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < menList.size(); j++)
			if (shoppingCart[i]->getName() == menList[j].getName()) {
				menList[j].addSales(shoppingCart[i]->getPurchaseQuantity());
				menList[j].decProductS(shoppingCart[i]->getPurchaseQuantity());
				menList[j].setPurchaseQuantity(0);
			}
		for (j = 0; j < foodList.size(); j++)
			if (shoppingCart[i]->getName() == foodList[j].getName()) {
				foodList[j].addSales(shoppingCart[i]->getPurchaseQuantity());
				foodList[j].decProductS(shoppingCart[i]->getPurchaseQuantity());
				foodList[j].setPurchaseQuantity(0);
			}
	}
	for (i = 0; i < shoppingCart.size(); i++) {
		shoppingCart[i]->setPayTime(systemTime);
		waitForDeliver.push_back(shoppingCart[i]);
		amountOfWFD++;
	}
	shoppingCart.clear();
	cout << "*****************************************************************************************" << endl;
	for (i = 0; partOfC[i] != -1 && i<MAXCATEGORIES; i++) {
		switch (partOfC[i]) {
		case 0:
			buffer.clear();
			splitString(cosmeticList[0].getOffer(), buffer, " ");
			printf("美妆区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠金额为：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动";
			cout << endl;
			if (cosmeticList[0].getDiscount()) {
				partOfSum[i] *= cosmeticList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", cosmeticList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 1:
			buffer.clear();
			splitString(sportsList[0].getOffer(), buffer, " ");
			printf("运动户外区总金额（含邮费）：%.2f\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (sportsList[0].getDiscount()) {
				partOfSum[i] *= sportsList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", sportsList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 2:
			buffer.clear();
			splitString(articlesOfDUList[0].getOffer(), buffer, " ");
			printf("百货区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (articlesOfDUList[0].getDiscount()) {
				partOfSum[i] *= articlesOfDUList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", articlesOfDUList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 3:
			buffer.clear();
			splitString(womenList[0].getOffer(), buffer, " ");
			printf("女装区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (womenList[0].getDiscount()) {
				partOfSum[i] *= womenList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", womenList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 4:
			buffer.clear();
			splitString(menList[0].getOffer(), buffer, " ");
			printf("男装区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (menList[0].getDiscount()) {
				partOfSum[i] *= menList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", menList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
			break;
		case 5:
			buffer.clear();
			splitString(foodList[0].getOffer(), buffer, " ");
			printf("食品区总金额（含邮费）：%.2f\t\t", partOfSum[i]);
			if (atoi(buffer[0].c_str())) {
				if (partOfSum[i] < atoi(buffer[0].c_str()))
					cout << "不符合满减条件！";
				else {
					j = (int)partOfSum[i];
					k = atoi(buffer[0].c_str());
					a = j / k;
					partOfSum[i] -= a*atoi(buffer[1].c_str());
					printf("符合满减条件，优惠后总金额：%.2f", partOfSum[i]);
				}
			}
			else
				cout << "无满减活动！";
			cout << endl;
			if (foodList[0].getDiscount()) {
				partOfSum[i] *= foodList[0].getDiscount() / 10;
				printf("\t\t\t\t\t品类折扣为%.2f折，最终金额为：%.2f\n", foodList[0].getDiscount(), partOfSum[i]);
			}
			else
				printf("\t\t\t\t\t无品类折扣，最终金额为：%.2f\n", partOfSum[i]);
		}
	}
	for (i = 0; partOfSum[i] && i<MAXCATEGORIES; i++)
		sum += partOfSum[i];
	printf("\n\t\t\t\t\t\t\t\t合计（含邮费）：%.2f元\n", sum);
	cout << "..." << endl;
	Sleep(3000);
	cout << "支付成功！" << endl;
	amountOfSC = 0;
	return sum;
}

void User::setAmountOfWFD(int amount) {
	amountOfWFD = amount;
}

vector<Product*> User::getWFD() {
	return waitForDeliver;
}

int User::getAmountOfWFD() {
	return amountOfWFD;
}

void User::priWFD() {
	int i;
	int month, day;
	cout << "******************************************************************************************" << endl;
	cout << ">>*                                       待发货                                       *<<" << endl;
	cout << "******************************************************************************************" << endl;
	for (i = 0; i < amountOfWFD; i++) {
		cout << i + 1 << ".";
		waitForDeliver[i]->priProduct();
		month = (int)waitForDeliver[i]->getPayTime();
		day = (int)((waitForDeliver[i]->getPayTime() - month) * 100);
		cout << "付款时间：" << month << " - " << day << endl;
		if (i < amountOfWFD - 1)
			cout << endl;
	}
}

void User::updateWFD() {
	int i;
	for (i = 0; i < amountOfWFD; i++) {
		if (systemTime - waitForDeliver[i]->getPayTime() > 0.03) {
			waitForSignature.push_back(waitForDeliver[i]);
			waitForSignature[waitForSignature.size() - 1]->setDeliveryTime(systemTime);
			waitForDeliver.erase(waitForDeliver.begin() + i);
			i--;
			amountOfWFD--;
			amountOfWFS++;
		}
	}
}

void User::setAmountOfWFS(int amount) {
	amountOfWFS = amount;
}

vector<Product*> User::getWFS() {
	return waitForSignature;
}

int User::getAmountOfWFS() {
	return amountOfWFS;
}

void User::priWFS() {
	int i;
	int month, day;
	cout << "******************************************************************************************" << endl;
	cout << ">>*                                       待收货                                       *<<" << endl;
	cout << "******************************************************************************************" << endl;
	for (i = 0; i < amountOfWFS; i++) {
		cout << i + 1 << ".";
		waitForSignature[i]->priProduct();
		month = (int)waitForSignature[i]->getPayTime();
		day = (int)((waitForSignature[i]->getDeliveryTime() - month) * 100);
		cout << "发货时间：" << month << " - " << day << endl;
		if (i < amountOfWFS - 1)
			cout << endl;
	}
}

void User::confirmSignature() {
	unsigned int i,j;
	string choice;
	vector<string> choices;
	vector<Product *>::iterator iter;
	cout << endl << "请依次输入确认收货的商品序号，以空格为分界符：" << endl;
	getline(cin, choice);
	getline(cin, choice);
	splitString(choice, choices, " ");
	for (i = 0; i < choices.size(); i++) {
		iter = waitForSignature.begin();
		for (j = 0; j < atoi(choices[i].c_str()) - 1 - i; j++)
			iter++;
		waitForSignature.erase(iter);
	}
	cout << "..." << endl;
	Sleep(3000);
	cout << "确认收货成功！" << endl;
	amountOfWFS -= choices.size();
}

void User::priUser() {
	cout << "会员名：" << username << endl;
	if (shippingAddress.size()) {
		cout << ">>* 默认收货地址 *<<" << endl;
		defaultAddress.priAddress();
	}
	cout << endl;
	if (amountOfWFD)
		cout << "您有 " << amountOfWFD << " 件宝贝 正等待卖家发货(づ￣ 3￣)づ" << endl;
	else
		cout << "您无宝贝等待发货~" << endl;
	if (amountOfWFS)
		cout << "您有 " << amountOfWFS << " 件宝贝 正不分昼夜地奔向目的地(づ￣ 3￣)づ" << endl;
	else
		cout << "您无宝贝等待签收~" << endl;
	if (amountOfSC)
		cout << "您有 " << amountOfSC << " 件宝贝 正躺在购物车(づ￣ 3￣)づ" << endl << endl;
	else
		cout << "您的购物车空空如也，快去选购吧！(づ￣ 3￣)づ" << endl << endl;
}

User::User() {
	amountOfSA = 0;
	amountOfSC = 0;
	amountOfWFS = 0;
	amountOfWFD = 0;
	username = "小可爱";
}

User::~User() {

}