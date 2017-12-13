#include "E-commercePlatform.h"

														
vector<Cosmetic> cosmeticList;
vector<SportsOutdoor> sportsList;
vector<ArticlesOfDailyUse> articlesOfDUList;
vector<Women> womenList;
vector<Men> menList;
vector<Food> foodList;
vector<User> userList;
double systemTime = 0;							//系统时间
int pastDay = 0;
extern ifstream ifp;							//输入文件流
extern ofstream ofp;							//输出文件流

void readProducts();
void readUsers();
void readSystemTime();
void incSystemTime();
void writeSystemTime();

int main() {
	unsigned int i;
	unsigned int userTmp;
	int choice;
	unsigned int order;
	int amount;
	unsigned int page;
	clock_t startTime, curTime;
	double pastTime = 0;
	string::size_type index;
	vector<int> productTmp;
	string phoneNumber, password;
	string productName;
	string productC;
	User user;
	readSystemTime();
	Product* product;
	readProducts();
	User::readAnnotation();
	readUsers();
	cout << "******************************************************************************************" << endl;
	cout << ">>*                                       电商平台                                     *<<" << endl;
	cout << "******************************************************************************************" << endl;
	cout << "欢迎欢迎(づ￣ 3￣)づ，请您先进行以下操作：" << endl;
	cout << "1.登陆\t2.注册" << endl << endl;
	cout << "请输入您的选择：" << endl;
	while (1){
		cin >> choice;
		if (choice == 1) {
			cout << "******************************************************************************************" << endl;
			cout << "请输入您的手机号码：" << endl;
			cin >> phoneNumber;
			log:
			cout << "请输入您的密码：" << endl;
			cin >> password;
			for (userTmp = 0; userTmp < userList.size(); userTmp++)
				if (userList[userTmp].getPhoneNumber() == phoneNumber)
					break;
			if (userTmp == userList.size()) {
				cout << endl << "不存在该用户！" << endl;
				cout << endl << "请您：" << endl;
				cout << "1.重新登陆\t2.注册" << endl << endl;
				cout << "请输入您的选择：" << endl;
				continue;
			}
			if (userList[userTmp].getPassword() != password) {
				cout << endl << "登录失败，账号或密码错误！" << endl;
				cout << endl << "请您：" << endl;
				cout << "1.重新登陆\t2.注册" << endl << endl;
				cout << "请输入您的选择：" << endl;
				continue;
			}
			cout << endl << "登陆成功！" << endl;
			break;
		}
		else if (choice == 2) {
			cout << "******************************************************************************************" << endl;
			cout << "请输入您的常用手机号码：" << endl;
			while (1) {
				cout << "+ 86 ";
				cin >> phoneNumber;
				for (userTmp = 0; userTmp < userList.size(); userTmp++)
					if (userList[userTmp].getPhoneNumber() == phoneNumber) {
						cout << endl << "此用户已存在！请您直接登录：" << endl << endl;
						goto log;
					}
				if (phoneNumber.length() != 11)
					cout << endl << "手机号有误！请输入正确信息：" << endl;
				else {
					user.setPhoneNumber(phoneNumber);
					break;
				}
			}
			cout << "请输入您的密码：" << endl;
			while (1) {
				cin >> password;
				if (password.length() < 6)
					cout << endl << "密码长度至少为6位！请您重新输入：" << endl;
				else {
					user.setPassword(password);
					break;
				}
			}
			userList.push_back(user);
			cout << endl << "注册成功！" << endl;
			break;
		}
		else {
			cout << endl << "操作序号输入有误！请您重新输入：" << endl;
		}
	}
	for (userTmp = 0; userTmp < userList.size(); userTmp++)
		if (userList[userTmp].getPhoneNumber() == phoneNumber)
			break;
	startTime = clock();
	start1:
	cout << "******************************************************************************************" << endl;
	cout << "您可选择：" << endl;
	cout << "1.淘我喜欢\t2.购物车（" << userList[userTmp].getAmountOfSC() << "）\n3.关于我\t4.退出系统" << endl << endl;
	cout << "请输入您的选择：" << endl;
	while (1) {
		cin >> choice;
		switch (choice){
		case 1:
			while (1) {
			start2:
				productTmp.clear();
				cout << "******************************************************************************************" << endl;
				cout << "您可选择：" << endl;
				cout << "1.搜索宝贝\t2.分区浏览\t3.返回主菜单" << endl << endl;
				cout << "请输入您的选择：" << endl;
				while (1) {
					cin >> choice;
					if (choice == 1) {
						cout << "******************************************************************************************" << endl;
						cout << "请输入宝贝名称：" << endl;
						cin >> productName;
						cout << "..." << endl;
						Sleep(2000);
						for (i = 0; i < cosmeticList.size(); i++) {
							index = cosmeticList[i].getName().find(productName);
							if (index != string::npos)
								productTmp.push_back(i);
						}
						if (!productTmp.size()) {
							for (i = 0; i < sportsList.size(); i++) {
								index = sportsList[i].getName().find(productName);
								if (index != string::npos)
									productTmp.push_back(i);
							}
							if (!productTmp.size()) {
								for (i = 0; i < articlesOfDUList.size(); i++) {
									index = articlesOfDUList[i].getName().find(productName);
									if (index != string::npos)
										productTmp.push_back(i);
								}
								if (!productTmp.size()) {
									for (i = 0; i < womenList.size(); i++) {
										index = womenList[i].getName().find(productName);
										if (index != string::npos) 
											productTmp.push_back(i);
									}
									if (!productTmp.size()) {
										for (i = 0; i < menList.size(); i++) {
											index = menList[i].getName().find(productName);
											if (index != string::npos)
												productTmp.push_back(i);
										}
										if (!productTmp.size()) {
											for (i = 0; i < foodList.size(); i++) {
												index = foodList[i].getName().find(productName);
												if (index != string::npos)
													productTmp.push_back(i);
											}
											if (!productTmp.size()) {
												cout << endl << "无相关宝贝！" << endl;									//进行推荐
												goto start2;
											}
											else
												productC = "food";
										}
										else
											productC = "men";
									}
									else
										productC = "women";
								}
								else
									productC = "articlesOfDU";

							}
							else
								productC = "sports";
						}
						else
							productC = "cosmetic";
						if (productC == "cosmetic") {
							if (productTmp.size() <= AMOUNTOFITEMSOFPAGE) {
								cosmeticStart3:
								cout << "******************************************************************************************" << endl;
								for (i = 0; i < productTmp.size(); i++) {
									cout << i + 1 << ".";
									cosmeticList[productTmp[i]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (order <= productTmp.size()) {
												cosmeticList[productTmp[order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														cosmeticList[productTmp[order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														cosmeticList[productTmp[order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &cosmeticList[productTmp[order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto cosmeticStart3;
													}
													else if (choice == 2)
														goto cosmeticStart3;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2) 
										goto start2;
									else
										cout << endl << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
							else {
								for (page = 1; page * AMOUNTOFITEMSOFPAGE < productTmp.size(); page++) {
									cosmeticStart5:
									if (page * AMOUNTOFITEMSOFPAGE >= productTmp.size())
										break;
									cout << "******************************************************************************************" << endl;
									if (page == 1) {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto cosmeticStart5;
															}
															else if (choice == 2) {
																goto cosmeticStart5;
															}
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page++;
												goto cosmeticStart5;
											}
											else if (choice == 3)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
									else {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto cosmeticStart5;
															}
															else if (choice == 2) {
																goto cosmeticStart5;
															}
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page--;
												goto cosmeticStart5;
											}
											else if (choice == 3) {
												page++;
												goto cosmeticStart5;
											}
											else if (choice == 4)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
								}
								page--;
								cosmeticStart4:
								cout << "******************************************************************************************" << endl;
								for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= productTmp.size(); i++) {
									cout << i << ".";
									cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * page + i - 1]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (AMOUNTOFITEMSOFPAGE * page + order <= productTmp.size()) {
												cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &cosmeticList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto cosmeticStart4;
													}
													else if (choice == 2) 
														goto cosmeticStart4;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto cosmeticStart5;
									else if (choice == 3)
										goto start2;
									else
										cout << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
						}
						else if (productC == "sports") {
							if (productTmp.size() <= AMOUNTOFITEMSOFPAGE) {
								sportsStart3:
								cout << "******************************************************************************************" << endl;
								for (i = 0; i < productTmp.size(); i++) {
									cout << i + 1 << ".";
									sportsList[productTmp[i]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (order <= productTmp.size()) {
												sportsList[productTmp[order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														sportsList[productTmp[order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														sportsList[productTmp[order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &sportsList[productTmp[order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto sportsStart3;
													}
													else if (choice == 2)
														goto sportsStart3;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2) 
										goto start2;
									else
										cout << endl << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
							else {
								for (page = 1; page * AMOUNTOFITEMSOFPAGE < productTmp.size(); page++) {
									sportsStart5:
									if (page * AMOUNTOFITEMSOFPAGE >= productTmp.size())
										break;
									cout << "******************************************************************************************" << endl;
									if (page == 1) {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto sportsStart5;
															}
															else if (choice == 2)
																goto sportsStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page++;
												goto sportsStart5;
											}
											else if (choice == 3)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
									else {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &sportsList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto sportsStart5;
															}
															else if (choice == 2)
																goto sportsStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page--;
												goto sportsStart5;
											}
											else if (choice == 3) {
												page++;
												goto sportsStart5;
											}
											else if (choice == 4)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
								}
								page--;
								sportsStart4:
								cout << "******************************************************************************************" << endl;
								for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= productTmp.size(); i++) {
									cout << i << ".";
									sportsList[productTmp[AMOUNTOFITEMSOFPAGE * page + i - 1]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (AMOUNTOFITEMSOFPAGE * page + order <= productTmp.size()) {
												sportsList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														sportsList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														sportsList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &sportsList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto sportsStart4;
													}
													else if (choice == 2)
														goto sportsStart4;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto sportsStart5;
									else if (choice == 3)
										goto start2;
									else
										cout << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
						}
						else if (productC == "articlesOfDU") {
							if (productTmp.size() <= AMOUNTOFITEMSOFPAGE) {
								articlesOfDUStart3:
								cout << "******************************************************************************************" << endl;
								for (i = 0; i < productTmp.size(); i++) {
									cout << i + 1 << ".";
									articlesOfDUList[productTmp[i]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (order <= productTmp.size()) {
												articlesOfDUList[productTmp[order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														articlesOfDUList[productTmp[order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														articlesOfDUList[productTmp[order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &articlesOfDUList[productTmp[order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto articlesOfDUStart3;
													}
													else if (choice == 2)
														goto articlesOfDUStart3;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto start2;
									else
										cout << endl << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
							else {
								for (page = 1; page * AMOUNTOFITEMSOFPAGE < productTmp.size(); page++) {
									articlesOfDUStart5:
									if (page * AMOUNTOFITEMSOFPAGE >= productTmp.size())
										break;
									cout << "******************************************************************************************" << endl;
									if (page == 1) {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto articlesOfDUStart5;
															}
															else if (choice == 2)
																goto articlesOfDUStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page++;
												goto articlesOfDUStart5;
											}
											else if (choice == 3)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
									else {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto articlesOfDUStart5;
															}
															else if (choice == 2)
																goto articlesOfDUStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page--;
												goto articlesOfDUStart5;
											}
											else if (choice == 3) {
												page++;
												goto articlesOfDUStart5;
											}
											else if (choice == 4)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
								}
								page--;
								articlesOfDUStart4:
								cout << "******************************************************************************************" << endl;
								for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= productTmp.size(); i++) {
									cout << i << ".";
									articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * page + i - 1]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (AMOUNTOFITEMSOFPAGE * page + order <= productTmp.size()) {
												articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &articlesOfDUList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto articlesOfDUStart4;
													}
													else if (choice == 2)
														goto articlesOfDUStart4;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto articlesOfDUStart5;
									else if (choice == 3)
										goto start2;
									else
										cout << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
						}
						else if (productC == "women") {
							if (productTmp.size() <= AMOUNTOFITEMSOFPAGE) {
								womenStart3:
								cout << "******************************************************************************************" << endl;
								for (i = 0; i < productTmp.size(); i++) {
									cout << i + 1 << ".";
									womenList[productTmp[i]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (order <= productTmp.size()) {
												womenList[productTmp[order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														womenList[productTmp[order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														womenList[productTmp[order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &womenList[productTmp[order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto womenStart3;
													}
													else if (choice == 2)
														goto womenStart3;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto start2;
									else
										cout << endl << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
							else {
								for (page = 1; page * AMOUNTOFITEMSOFPAGE < productTmp.size(); page++) {
									womenStart5:
									if (page * AMOUNTOFITEMSOFPAGE >= productTmp.size())
										break;
									cout << "******************************************************************************************" << endl;
									if (page == 1) {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto womenStart5;
															}
															else if (choice == 2)
																goto womenStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page++;
												goto womenStart5;
											}
											else if (choice == 3)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
									else {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &womenList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto womenStart5;
															}
															else if (choice == 2)
																goto womenStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page--;
												goto womenStart5;
											}
											else if (choice == 3) {
												page++;
												goto womenStart5;
											}
											else if (choice == 4)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
								}
								page--;
								womenStart4:
								cout << "******************************************************************************************" << endl;
								for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= productTmp.size(); i++) {
									cout << i << ".";
									womenList[productTmp[AMOUNTOFITEMSOFPAGE * page + i - 1]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (AMOUNTOFITEMSOFPAGE * page + order <= productTmp.size()) {
												womenList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														womenList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														womenList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &womenList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto womenStart4;
													}
													else if (choice == 2)
														goto womenStart4;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto womenStart5;
									else if (choice == 3)
										goto start2;
									else
										cout << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
						}
						else if (productC == "men") {
							if (productTmp.size() <= AMOUNTOFITEMSOFPAGE) {
								menStart3:
								cout << "******************************************************************************************" << endl;
								for (i = 0; i < productTmp.size(); i++) {
									cout << i + 1 << ".";
									menList[productTmp[i]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (order <= productTmp.size()) {
												menList[productTmp[order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														menList[productTmp[order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														menList[productTmp[order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &menList[productTmp[order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto menStart3;
													}
													else if (choice == 2)
														goto menStart3;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto start2;
									else
										cout << endl << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
							else {
								for (page = 1; page * AMOUNTOFITEMSOFPAGE < productTmp.size(); page++) {
									menStart5:
									if (page * AMOUNTOFITEMSOFPAGE >= productTmp.size())
										break;
									cout << "******************************************************************************************" << endl;
									if (page == 1) {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto menStart5;
															}
															else if (choice == 2)
																goto menStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page++;
												goto menStart5;
											}
											else if (choice == 3)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
									else {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &menList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto menStart5;
															}
															else if (choice == 2)
																goto menStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page--;
												goto menStart5;
											}
											else if (choice == 3) {
												page++;
												goto menStart5;
											}
											else if (choice == 4)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
								}
								page--;
								menStart4:
								cout << "******************************************************************************************" << endl;
								for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= productTmp.size(); i++) {
									cout << i << ".";
									menList[productTmp[AMOUNTOFITEMSOFPAGE * page + i - 1]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (AMOUNTOFITEMSOFPAGE * page + order <= productTmp.size()) {
												menList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														menList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														menList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &menList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto menStart4;
													}
													else if (choice == 2)
														goto menStart4;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto menStart5;
									else if (choice == 3)
										goto start2;
									else
										cout << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
						}
						else if (productC == "food") {
							if (productTmp.size() <= AMOUNTOFITEMSOFPAGE) {
								foodStart3:
								cout << "******************************************************************************************" << endl;
								for (i = 0; i < productTmp.size(); i++) {
									cout << i + 1 << ".";
									foodList[productTmp[i]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (order <= productTmp.size()) {
												foodList[productTmp[order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														foodList[productTmp[order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														foodList[productTmp[order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &foodList[productTmp[order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto foodStart3;
													}
													else if (choice == 2)
														goto foodStart3;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto start2;
									else
										cout << endl << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
							else {
								for (page = 1; page * AMOUNTOFITEMSOFPAGE < productTmp.size(); page++) {
									foodStart5:
									if (page * AMOUNTOFITEMSOFPAGE >= productTmp.size())
										break;
									cout << "******************************************************************************************" << endl;
									if (page == 1) {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto foodStart5;
															}
															else if (choice == 2)
																goto foodStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page++;
												goto foodStart5;
											}
											else if (choice == 3)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
									else {
										for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
											cout << i << ".";
											foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1]].priProductInList();
										}
										cout << "您可进行的操作有：" << endl;
										cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
										cout << "请输入您的选择：" << endl;
										while (1) {
											cin >> choice;
											if (choice == 1) {
												cout << endl << "请输入商品序号：" << endl;
												cin >> order;
												while (1) {
													if (order <= AMOUNTOFITEMSOFPAGE) {
														foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priDetailedProduct();
														cout << "您可进行的操作有：" << endl;
														cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
														cout << "请输入您的选择：" << endl;
														while (1) {
															cin >> choice;
															if (choice == 1) {
																foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].priInfo();
																cout << endl << "请输入购买数量：" << endl;
																cin >> amount;
																foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]].setPurchaseQuantity(amount,"repeat");
																product = &foodList[productTmp[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1]];
																userList[userTmp].addShoppingCart(product);
																cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																goto foodStart5;
															}
															else if (choice == 2)
																goto foodStart5;
															else
																cout << endl << "操作序号输入有误！请您重新输入：" << endl;
														}
													}
													else {
														cout << endl << "商品序号输入有误！请您重新输入：" << endl;
														cin >> order;
													}
												}
											}
											else if (choice == 2) {
												page--;
												goto foodStart5;
											}
											else if (choice == 3) {
												page++;
												goto foodStart5;
											}
											else if (choice == 4)
												goto start2;
											else
												cout << "操作序号输入有误！请您重新输入：" << endl;
										}
									}
								}
								page--;
								foodStart4:
								cout << "******************************************************************************************" << endl;
								for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= productTmp.size(); i++) {
									cout << i << ".";
									foodList[productTmp[AMOUNTOFITEMSOFPAGE * page + i - 1]].priProductInList();
								}
								cout << "您可进行的操作有：" << endl;
								cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
								cout << "请输入您的选择：" << endl;
								while (1) {
									cin >> choice;
									if (choice == 1) {
										cout << endl << "请输入商品序号：" << endl;
										cin >> order;
										while (1) {
											if (AMOUNTOFITEMSOFPAGE * page + order <= productTmp.size()) {
												foodList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priDetailedProduct();
												cout << "您可进行的操作有：" << endl;
												cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
												cout << "请输入您的选择：" << endl;
												while (1) {
													cin >> choice;
													if (choice == 1) {
														foodList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].priInfo();
														cout << endl << "请输入购买数量：" << endl;
														cin >> amount;
														foodList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]].setPurchaseQuantity(amount,"repeat");
														product = &foodList[productTmp[AMOUNTOFITEMSOFPAGE * page + order - 1]];
														userList[userTmp].addShoppingCart(product);
														cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
														goto foodStart4;
													}
													else if (choice == 2)
														goto foodStart4;
													else
														cout << endl << "操作序号输入有误！请您重新输入：" << endl;
												}
											}
											else {
												cout << endl << "商品序号输入有误！请您重新输入：" << endl;
												cin >> order;
											}
										}
									}
									else if (choice == 2)
										goto foodStart5;
									else if (choice == 3)
										goto start2;
									else
										cout << "操作序号输入有误！请您重新输入：" << endl;
								}
							}
						}
					}
					else if (choice == 2) {
						start:
						cout << "******************************************************************************************" << endl;
						cout << "商品分类：" << endl;
						cout << "1.美妆\t\t2.运动户外\t3.百货" << endl;
						cout << "4.女装\t\t5.男装\t\t6.食品" << endl;
						cout << "7.返回上一级菜单" << endl << endl;
						cout << "请输入您的选择：" << endl;
						while (1) {
							cin >> choice;
							switch (choice){
							case 1:
								if (cosmeticList.size() <= AMOUNTOFITEMSOFPAGE) {
									cStart3:
									cout << "******************************************************************************************" << endl;
									for (i = 0; i < cosmeticList.size(); i++) {
										cout << i + 1 << ".";
										cosmeticList[i].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (order <= cosmeticList.size()) {
													cosmeticList[order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															cosmeticList[order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															cosmeticList[order - 1].setPurchaseQuantity(amount,"repeat");
															product = &cosmeticList[order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto cStart3;
														}
														else if (choice == 2) 
															goto cStart3;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto start;
										else
											cout << endl << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
								else {
									for (page = 1; page * AMOUNTOFITEMSOFPAGE < cosmeticList.size(); page++) {
										cStart5:
										if (page * AMOUNTOFITEMSOFPAGE >= cosmeticList.size())
											break;
										cout << "******************************************************************************************" << endl;
										if (page == 1) {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto cStart5;
																}
																else if (choice == 2)
																	goto cStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page++;
													goto cStart5;
												}
												else if (choice == 3)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
										else {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &cosmeticList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto cStart5;
																}
																else if (choice == 2)
																	goto cStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page--;
													goto cStart5;
												}
												else if (choice == 3) {
													page++;
													goto cStart5;
												}
												else if (choice == 4)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
									}
									page--;
									cStart4:
									cout << "******************************************************************************************" << endl;
									for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= cosmeticList.size(); i++) {
										cout << i << ".";
										cosmeticList[AMOUNTOFITEMSOFPAGE * page + i - 1].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (AMOUNTOFITEMSOFPAGE * page + order <= cosmeticList.size()) {
													cosmeticList[AMOUNTOFITEMSOFPAGE * page + order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															cosmeticList[AMOUNTOFITEMSOFPAGE * page + order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															cosmeticList[AMOUNTOFITEMSOFPAGE * page + order - 1].setPurchaseQuantity(amount,"repeat");
															product = &cosmeticList[AMOUNTOFITEMSOFPAGE * page + order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto cStart4;
														}
														else if (choice == 2)
															goto cStart4;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto cStart5;
										else if (choice == 3)
											goto start;
										else
											cout << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
							case 2:
								if (sportsList.size() <= AMOUNTOFITEMSOFPAGE) {
									sStart3:
									cout << "******************************************************************************************" << endl;
									for (i = 0; i < sportsList.size(); i++) {
										cout << i + 1 << ".";
										sportsList[i].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (order <= sportsList.size()) {
													sportsList[order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															sportsList[order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															sportsList[order - 1].setPurchaseQuantity(amount,"repeat");
															product = &sportsList[order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto sStart3;
														}
														else if (choice == 2)
															goto sStart3;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto start;
										else
											cout << endl << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
								else {
									for (page = 1; page * AMOUNTOFITEMSOFPAGE < sportsList.size(); page++) {
										sStart5:
										if (page * AMOUNTOFITEMSOFPAGE >= sportsList.size())
											break;
										cout << "******************************************************************************************" << endl;
										if (page == 1) {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto sStart5;
																}
																else if (choice == 2)
																	goto sStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page++;
													goto sStart5;
												}
												else if (choice == 3)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
										else {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &sportsList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto sStart5;
																}
																else if (choice == 2)
																	goto sStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page--;
													goto sStart5;
												}
												else if (choice == 3) {
													page++;
													goto sStart5;
												}
												else if (choice == 4)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
									}
									page--;
									sStart4:
									cout << "******************************************************************************************" << endl;
									for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= sportsList.size(); i++) {
										cout << i << ".";
										sportsList[AMOUNTOFITEMSOFPAGE * page + i - 1].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (AMOUNTOFITEMSOFPAGE * page + order <= sportsList.size()) {
													sportsList[AMOUNTOFITEMSOFPAGE * page + order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															sportsList[AMOUNTOFITEMSOFPAGE * page + order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															sportsList[AMOUNTOFITEMSOFPAGE * page + order - 1].setPurchaseQuantity(amount,"repeat");
															product = &sportsList[AMOUNTOFITEMSOFPAGE * page + order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto sStart4;
														}
														else if (choice == 2)
															goto sStart4;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto sStart5;
										else if (choice == 3)
											goto start;
										else
											cout << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
							case 3:
								if (articlesOfDUList.size() <= AMOUNTOFITEMSOFPAGE) {
									aStart3:
									cout << "******************************************************************************************" << endl;
									for (i = 0; i < articlesOfDUList.size(); i++) {
										cout << i + 1 << ".";
										articlesOfDUList[i].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (order <= articlesOfDUList.size()) {
													articlesOfDUList[order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															articlesOfDUList[order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															articlesOfDUList[order - 1].setPurchaseQuantity(amount,"repeat");
															product = &articlesOfDUList[order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto aStart3;
														}
														else if (choice == 2)
															goto aStart3;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto start;
										else
											cout << endl << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
								else {
									for (page = 1; page * AMOUNTOFITEMSOFPAGE < articlesOfDUList.size(); page++) {
										aStart5:
										if (page * AMOUNTOFITEMSOFPAGE >= articlesOfDUList.size())
											break;
										cout << "******************************************************************************************" << endl;
										if (page == 1) {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto aStart5;
																}
																else if (choice == 2)
																	goto aStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page++;
													goto aStart5;
												}
												else if (choice == 3)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
										else {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &articlesOfDUList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto aStart5;
																}
																else if (choice == 2)
																	goto aStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page--;
													goto aStart5;
												}
												else if (choice == 3) {
													page++;
													goto aStart5;
												}
												else if (choice == 4)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
									}
									page--;
									aStart4:
									cout << "******************************************************************************************" << endl;
									for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= articlesOfDUList.size(); i++) {
										cout << i << ".";
										articlesOfDUList[AMOUNTOFITEMSOFPAGE * page + i - 1].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (AMOUNTOFITEMSOFPAGE * page + order <= articlesOfDUList.size()) {
													articlesOfDUList[AMOUNTOFITEMSOFPAGE * page + order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															articlesOfDUList[AMOUNTOFITEMSOFPAGE * page + order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															articlesOfDUList[AMOUNTOFITEMSOFPAGE * page + order - 1].setPurchaseQuantity(amount,"repeat");
															product = &articlesOfDUList[AMOUNTOFITEMSOFPAGE * page + order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto aStart4;
														}
														else if (choice == 2)
															goto aStart4;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto aStart5;
										else if (choice == 3)
											goto start;
										else
											cout << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
							case 4:
								if (womenList.size() <= AMOUNTOFITEMSOFPAGE) {
									wStart3:
									cout << "******************************************************************************************" << endl;
									for (i = 0; i < womenList.size(); i++) {
										cout << i + 1 << ".";
										womenList[i].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (order <= womenList.size()) {
													womenList[order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															womenList[order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															womenList[order - 1].setPurchaseQuantity(amount,"repeat");
															product = &womenList[order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto wStart3;
														}
														else if (choice == 2)
															goto wStart3;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto start;
										else
											cout << endl << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
								else {
									for (page = 1; page * AMOUNTOFITEMSOFPAGE < womenList.size(); page++) {
										wStart5:
										if (page * AMOUNTOFITEMSOFPAGE >= womenList.size())
											break;
										cout << "******************************************************************************************" << endl;
										if (page == 1) {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto wStart5;
																}
																else if (choice == 2)
																	goto wStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page++;
													goto wStart5;
												}
												else if (choice == 3)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
										else {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &womenList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto wStart5;
																}
																else if (choice == 2)
																	goto wStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page--;
													goto wStart5;
												}
												else if (choice == 3) {
													page++;
													goto wStart5;
												}
												else if (choice == 4)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
									}
									page--;
									wStart4:
									cout << "******************************************************************************************" << endl;
									for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= womenList.size(); i++) {
										cout << i << ".";
										womenList[AMOUNTOFITEMSOFPAGE * page + i - 1].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (AMOUNTOFITEMSOFPAGE * page + order <= womenList.size()) {
													womenList[AMOUNTOFITEMSOFPAGE * page + order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															womenList[AMOUNTOFITEMSOFPAGE * page + order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															womenList[AMOUNTOFITEMSOFPAGE * page + order - 1].setPurchaseQuantity(amount,"repeat");
															product = &womenList[AMOUNTOFITEMSOFPAGE * page + order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto wStart4;
														}
														else if (choice == 2)
															goto wStart4;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto wStart5;
										else if (choice == 3)
											goto start;
										else
											cout << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
							case 5:
								if (menList.size() <= AMOUNTOFITEMSOFPAGE) {
									mStart3:
									cout << "******************************************************************************************" << endl;
									for (i = 0; i < menList.size(); i++) {
										cout << i + 1 << ".";
										menList[i].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (order <= menList.size()) {
													menList[order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															menList[order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															menList[order - 1].setPurchaseQuantity(amount,"repeat");
															product = &menList[order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto mStart3;
														}
														else if (choice == 2)
															goto mStart3;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto start;
										else
											cout << endl << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
								else {
									for (page = 1; page * AMOUNTOFITEMSOFPAGE < menList.size(); page++) {
										mStart5:
										if (page * AMOUNTOFITEMSOFPAGE >= menList.size())
											break;
										cout << "******************************************************************************************" << endl;
										if (page == 1) {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												menList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															menList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	menList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	menList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &menList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto mStart5;
																}
																else if (choice == 2)
																	goto mStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page++;
													goto mStart5;
												}
												else if (choice == 3)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
										else {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												menList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															menList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	menList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	menList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &menList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto mStart5;
																}
																else if (choice == 2)
																	goto mStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page--;
													goto mStart5;
												}
												else if (choice == 3) {
													page++;
													goto mStart5;
												}
												else if (choice == 4)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
									}
									page--;
									mStart4:
									cout << "******************************************************************************************" << endl;
									for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= menList.size(); i++) {
										cout << i << ".";
										menList[AMOUNTOFITEMSOFPAGE * page + i - 1].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (AMOUNTOFITEMSOFPAGE * page + order <= menList.size()) {
													menList[AMOUNTOFITEMSOFPAGE * page + order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															menList[AMOUNTOFITEMSOFPAGE * page + order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															menList[AMOUNTOFITEMSOFPAGE * page + order - 1].setPurchaseQuantity(amount,"repeat");
															product = &menList[AMOUNTOFITEMSOFPAGE * page + order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto mStart4;
														}
														else if (choice == 2)
															goto mStart4;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto mStart5;
										else if (choice == 3)
											goto start;
										else
											cout << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
							case 6:
								if (foodList.size() <= AMOUNTOFITEMSOFPAGE) {
									fStart3:
									cout << "******************************************************************************************" << endl;
									for (i = 0; i < foodList.size(); i++) {
										cout << i + 1 << ".";
										foodList[i].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t2.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (order <= foodList.size()) {
													foodList[order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															foodList[order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															foodList[order - 1].setPurchaseQuantity(amount,"repeat");
															product = &foodList[order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto fStart3;
														}
														else if (choice == 2)
															goto fStart3;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto start;
										else
											cout << endl << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
								else {
									for (page = 1; page * AMOUNTOFITEMSOFPAGE < foodList.size(); page++) {
										fStart5:
										if (page * AMOUNTOFITEMSOFPAGE >= foodList.size())
											break;
										cout << "******************************************************************************************" << endl;
										if (page == 1) {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.下一页\t3.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto fStart5;
																}
																else if (choice == 2)
																	goto fStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page++;
													goto fStart5;
												}
												else if (choice == 3)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
										else {
											for (i = 1; i <= AMOUNTOFITEMSOFPAGE; i++) {
												cout << i << ".";
												foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + i - 1].priProductInList();
											}
											cout << "您可进行的操作有：" << endl;
											cout << "1.查阅商品信息\t\t2.上一页\t3.下一页\t4.返回上一级菜单" << endl << endl;
											cout << "请输入您的选择：" << endl;
											while (1) {
												cin >> choice;
												if (choice == 1) {
													cout << endl << "请输入商品序号：" << endl;
													cin >> order;
													while (1) {
														if (order <= AMOUNTOFITEMSOFPAGE) {
															foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priDetailedProduct();
															cout << "您可进行的操作有：" << endl;
															cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
															cout << "请输入您的选择：" << endl;
															while (1) {
																cin >> choice;
																if (choice == 1) {
																	foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].priInfo();
																	cout << endl << "请输入购买数量：" << endl;
																	cin >> amount;
																	foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1].setPurchaseQuantity(amount,"repeat");
																	product = &foodList[AMOUNTOFITEMSOFPAGE * (page - 1) + order - 1];
																	userList[userTmp].addShoppingCart(product);
																	cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
																	goto fStart5;
																}
																else if (choice == 2)
																	goto fStart5;
																else
																	cout << endl << "操作序号输入有误！请您重新输入：" << endl;
															}
														}
														else {
															cout << endl << "商品序号输入有误！请您重新输入：" << endl;
															cin >> order;
														}
													}
												}
												else if (choice == 2) {
													page--;
													goto fStart5;
												}
												else if (choice == 3) {
													page++;
													goto fStart5;
												}
												else if (choice == 4)
													goto start;
												else
													cout << "操作序号输入有误！请您重新输入：" << endl;
											}
										}
									}
									page--;
								fStart4:
									cout << "******************************************************************************************" << endl;
									for (i = 1; i + AMOUNTOFITEMSOFPAGE * page <= foodList.size(); i++) {
										cout << i << ".";
										foodList[AMOUNTOFITEMSOFPAGE * page + i - 1].priProductInList();
									}
									cout << "您可进行的操作有：" << endl;
									cout << "1.查阅商品信息\t\t2.上一页\t3.返回上一级菜单" << endl << endl;
									cout << "请输入您的选择：" << endl;
									while (1) {
										cin >> choice;
										if (choice == 1) {
											cout << endl << "请输入商品序号：" << endl;
											cin >> order;
											while (1) {
												if (AMOUNTOFITEMSOFPAGE * page + order <= foodList.size()) {
													foodList[AMOUNTOFITEMSOFPAGE * page + order - 1].priDetailedProduct();
													cout << "您可进行的操作有：" << endl;
													cout << "1.加入购物车\t2.返回上一级菜单" << endl << endl;
													cout << "请输入您的选择：" << endl;
													while (1) {
														cin >> choice;
														if (choice == 1) {
															foodList[AMOUNTOFITEMSOFPAGE * page + order - 1].priInfo();
															cout << endl << "请输入购买数量：" << endl;
															cin >> amount;
															foodList[AMOUNTOFITEMSOFPAGE * page + order - 1].setPurchaseQuantity(amount,"repeat");
															product = &foodList[AMOUNTOFITEMSOFPAGE * page + order - 1];
															userList[userTmp].addShoppingCart(product);
															cout << endl << "添加成功，在购物车等亲~(づ￣ 3￣)づ" << endl;
															goto fStart4;
														}
														else if (choice == 2)
															goto fStart4;
														else
															cout << endl << "操作序号输入有误！请您重新输入：" << endl;
													}
												}
												else {
													cout << endl << "商品序号输入有误！请您重新输入：" << endl;
													cin >> order;
												}
											}
										}
										else if (choice == 2)
											goto fStart5;
										else if (choice == 3)
											goto start;
										else
											cout << "操作序号输入有误！请您重新输入：" << endl;
									}
								}
							case 7:
									goto start2;
							default:
								cout << endl << "序号输入有误！请您重新输入：" << endl;
							}
						}
					}
					else if (choice == 3)
						goto start1;
					else
						cout << endl << "操作序号输入有误！请您重新输入：" << endl;
				}
			}
		case 2:
			curTime = clock();
			pastTime += (curTime - startTime) / (10 * CLOCKS_PER_SEC);
			pastDay = (int)pastTime;
			incSystemTime();
			pastTime -= pastDay;
			startTime = clock();
			userList[userTmp].updateWFD();
			start3:
			cout << "******************************************************************************************" << endl;
			if (userList[userTmp].priShoppingCart()) {
				cout << "您可进行的操作有：" << endl;
				cout << "1.全选结算\t2.部分结算\t3.修改购买数量" << endl;
				cout << "4.删除部分宝贝\t5.清空购物车\t6.返回主菜单" << endl << endl;
				cout << "请输入您的选择：" << endl;
				while (1) {
					cin >> choice;
					if (choice == 1) {
						curTime = clock();
						pastTime += (curTime - startTime) / (10 * CLOCKS_PER_SEC);
						pastDay = (int)pastTime;
						incSystemTime();
						pastTime -= pastDay;
						startTime = clock();
						userList[userTmp].updateWFD();
						userList[userTmp].clearALLShoppingCart();
						goto start3;
					}
					else if (choice == 2) {
						curTime = clock();
						pastTime += (curTime - startTime) / (10 * CLOCKS_PER_SEC);
						pastDay = (int)pastTime;
						incSystemTime();
						pastTime -= pastDay;
						startTime = clock();
						userList[userTmp].updateWFD();
						userList[userTmp].clearPartOfSC();
						goto start3;
					}
					else if (choice == 3) {
						userList[userTmp].modifyPurchaseQuantity();
						goto start3;
					}
					else if (choice == 4) {
						userList[userTmp].delPartOfSC();
						goto start3;
					}
					else if (choice == 5) {
						userList[userTmp].emptyShoppingCart();
						goto start3;
					}
					else if (choice == 6)
						goto start1;
					else
						cout << endl << "操作序号输入有误！请您重新输入：" << endl;
				}
			}
			else
				goto start1;
		case 3:
			start4:
			curTime = clock();
			pastTime += (curTime - startTime) / (10 * CLOCKS_PER_SEC);
			pastDay = (int)pastTime;
			incSystemTime();
			pastTime -= pastDay;
			startTime = clock();
			cout << "******************************************************************************************" << endl;
			userList[userTmp].updateWFD();
			userList[userTmp].priUser();
			cout << "您可进行的操作有：" << endl;
			cout << "1.修改默认地址\t2.修改收货地址\t3.增添收货地址\t4.删除收货地址" << endl;
			cout << "5.查看待发货("<<userList[userTmp].getAmountOfWFD()<<")\t6.查看待收货("<<userList[userTmp].getAmountOfWFS()<<")\t7.修改用户名\t8.修改密码" << endl;
			cout << "9.返回主菜单" << endl << endl;
			cout << "请输入您的选择：" << endl;
			while (1) {
				cin >> choice;
				if (choice == 1) {
					if (userList[userTmp].getShippingAddress().size()) {
						userList[userTmp].priShippingAddress();
						userList[userTmp].modifyDefaultAddress();
						userList[userTmp].priShippingAddress();
					}
					else
						cout << endl<<"您还没有收货地址，快去增添吧！(づ￣ 3￣)づ" << endl;
					goto start4;
				}
				else if (choice == 2) {
					if (userList[userTmp].getShippingAddress().size()) {
						userList[userTmp].priShippingAddress();
						userList[userTmp].modifyShippingAddress();
						userList[userTmp].priShippingAddress();
					}
					else
						cout << endl<<"您还没有收货地址，快去增添吧！(づ￣ 3￣)づ" << endl;
					goto start4;
				}
				else if (choice == 3) {
					if (userList[userTmp].getShippingAddress().size()) 
						userList[userTmp].priShippingAddress();
					userList[userTmp].addShippingAddress();
					userList[userTmp].priShippingAddress();
					goto start4;
				}
				else if (choice == 4) {
					userList[userTmp].priShippingAddress();
					userList[userTmp].delShippingAddress();
					userList[userTmp].priShippingAddress();
					goto start4;
				}
				else if (choice == 5) {
					if (userList[userTmp].getAmountOfWFD())
						userList[userTmp].priWFD();
					else
						cout << endl << "您无待发货商品~(づ￣ 3￣)づ" << endl;
					goto start4;
				}
				else if (choice == 6) {
					if (userList[userTmp].getAmountOfWFS()) {
						userList[userTmp].priWFS();
						cout << "******************************************************************************************" << endl;
						cout << "您可进行的操作有：" << endl;
						cout << "1.确认收货\t2.返回上一级菜单" << endl;
						cout << endl << "请输入您的选择：" << endl;
						while (1) {
							cin >> choice;
							if (choice == 1) {
								userList[userTmp].confirmSignature();
								goto start4;
							}
							else if (choice == 2)
								goto start4;
							else
								cout << endl << "操作序号输入有误！请您重新输入：" << endl;
						}
					}
					else {
						cout << endl << "您无待收货商品~(づ￣ 3￣)づ" << endl;
						goto start4;
					}
				}
				else if (choice == 7) {
					userList[userTmp].modifyName();
					goto start4;
				}
				else if (choice == 8) {
					userList[userTmp].modifyPassword();
					goto start4;
				}
				else if (choice == 9)
					goto start1;
				else
					cout << endl << "操作序号输入有误！请您重新输入：" << endl;
			}	
		case 4:
			goto end;
		default:
			cout << endl << "操作序号输入有误！请您重新输入：" << endl;
		}
	}
	end:
	cout << "******************************************************************************************" << endl;
	cout << ">>*                                    感谢您的使用                                    *<<" << endl;
	cout << "******************************************************************************************" << endl << endl;
	curTime = clock();
	pastTime += (curTime - startTime) / (10 * CLOCKS_PER_SEC);
	pastDay = (int)pastTime;
	incSystemTime();
	userList[userTmp].updateWFD();
	writeSystemTime();
	User::writeAnnotation();
	for (i = 0; i < userList.size(); i++) {
		userList[i].writeUser();
		if (i != userList.size() - 1)
			ofp << endl;
	}
	ofp.close();
	Product::writeAnnotation();
	for (i = 0; i < cosmeticList.size(); i++) {
		cosmeticList[i].writeProduct();
		ofp << endl;
	}
	for (i = 0; i < sportsList.size(); i++) {
		sportsList[i].writeProduct();
		ofp << endl;
	}
	for (i = 0; i < articlesOfDUList.size(); i++) {
		articlesOfDUList[i].writeProduct();
		ofp << endl;
	}
	for (i = 0; i < womenList.size(); i++) {
		womenList[i].writeProduct();
		ofp << endl;
	}
	for (i = 0; i < menList.size(); i++) {
		menList[i].writeProduct();
		ofp << endl;
	}
	for (i = 0; i < foodList.size(); i++) {
		foodList[i].writeProduct();
		ofp << endl;
	}
	ofp.close();
	system("pause");
	return 0;
}

void splitString(string s, vector<string>& d, string c) {
	string::size_type index1, index2;
	index2 = s.find(c);
	index1 = 0;
	while (string::npos != index2) {
		d.push_back(s.substr(index1, index2 - index1));
		index1 = index2 + c.length();
		index2 = s.find(c, index1);
	}
	if (index1 != s.length())
		d.push_back(s.substr(index1));
}

void readProducts(){
	int i;
	string buffer;
	Cosmetic cosmetic;
	SportsOutdoor sports;
	ArticlesOfDailyUse articlesOfDU;
	Women women;
	Men men;
	Food food;
	Product::readCategories();
	Product::readAnnotation();
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
	while (!ifp.eof()) {
		getline(ifp, buffer);
		if (buffer == "美妆") {
			cosmetic.readProduct();
			cosmeticList.push_back(cosmetic);
		}
		else if (buffer == "运动户外") {
			sports.readProduct();
			sportsList.push_back(sports);
		}
		else if (buffer == "百货") {
			articlesOfDU.readProduct();
			articlesOfDUList.push_back(articlesOfDU);
		}
		else if (buffer == "女装") {
			women.readProduct();
			womenList.push_back(women);
		}
		else if (buffer == "男装") {
			men.readProduct();
			menList.push_back(men);
		}
		else if (buffer == "食品") {
			food.readProduct();
			foodList.push_back(food);
		}
		else
			break;
		getline(ifp, buffer);
	}
	ifp.close();
}

void readUsers(){
	int amount;
	string buffer;
	User user;
	while (!ifp.eof()) {
		getline(ifp, buffer);
		user.setUsername(buffer);
		ifp >> amount;
		user.setAmountOfSA(amount);
		getline(ifp, buffer);
		user.readAddress();
		getline(ifp, buffer);
		user.setPhoneNumber(buffer);
		getline(ifp, buffer);
		user.setPassword(buffer);
		ifp >> amount;
		user.setAmountOfSC(amount);
		getline(ifp, buffer);
		user.read();
		userList.push_back(user);
		getline(ifp, buffer);
	}
	ifp.close();
}

void readSystemTime() {
	ifp.open("File/System Time.txt", ios::in);
	ifp >> systemTime;
	ifp.close();
}

void incSystemTime() {
	int month = (int)systemTime;
	double day = (systemTime - (int)systemTime) * 100;
	if (month == 2 && day + pastDay > 28)
		systemTime = 3 + (day + pastDay - 28) / 100;
	else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && (day + pastDay > 31)) {
		if (month == 12)
			systemTime = 1 + (day + pastDay - 31) / 100;
		else
			systemTime = ++month + (day + pastDay - 31) / 100;
	}
	else if ((month == 4 || month == 6 || month == 9 || month == 11) && (day + pastDay > 30))
		systemTime = ++month + (day + pastDay - 30) / 100;
	else
		systemTime += pastDay / 100.0;
}

void writeSystemTime() {
	ofp.open("File/System Time.txt", ios::out);
	ofp << setiosflags(ios::fixed) << setprecision(2) << systemTime;
	ofp.close();
}