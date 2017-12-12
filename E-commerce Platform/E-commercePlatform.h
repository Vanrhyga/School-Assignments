#pragma once
#include<stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdlib.h>
#include<windows.h>
#include<iomanip>
using namespace std;


#define AMOUNTOFITEMSOFPAGE 4
#define MAXCATEGORIES 10


class Address {
private:
	string consignee;						//收货人
	string contactNumber;					//联系电话
	string detailedAddress;					//具体地址
public:
	void readAddress();						//从文件读取地址信息
	void writeAddress();					//向文件写入地址信息
	void setConsignee(string);				//设置收货人
	void setContactNumber(string);			//设置联系电话
	void setDetailedAddress(string);		//设置具体地址
	string getContactNumber();				//获得联系电话
	void priAddress();						//打印地址
	Address();
	~Address();
};

class Product {
private:
	static vector<string> categories;		//分区
	static vector<double> discountOfC;		//分区折扣
	static vector<string> offerOfC;			//分区满减
	static int amountOfC;					//分区总数量
	string name;							//产品名称
	double originalPrice;					//产品原价
	double currentPrice;					//产品现价
	string category;						//产品类别
	double expressFee;						//快递费
	int sales;								//销量
	string storeName;						//店家名称
	string storeAddress;					//店家地址
	int productSurplus;						//产品剩余量
	int purchaseQuantity;					//购买数量
	int state;								//产品状态（1：待发货，2：待收货）
	double payTime;							//付款时间
	double deliveryTime;					//发货时间
	double discount;						//品类折扣系数
	string offer;							//满减
	string intro;							//描述
public:
	static void readCategories();			//读取分区信息
	static void readAnnotation();			//跳过注释信息
	static void writeAnnotation();			//向文件写入注释信息
	void readProduct();						//读取产品信息
	void readForUser();						//读取用户产品信息
	void writeProduct();					//向文件写入产品信息
	void writeForUser();					//向文件写入用户产品信息
	virtual void setCategory(string);		//设置产品类别
	void setPurchaseQuantity(int);			//设置购买数量
	void setPurchaseQuantity(int, string);	//设置购买数量
	void setState(int);						//设置产品状态
	void setPayTime(double);				//设置付款时间
	void setDeliveryTime(double);			//设置发货时间
	void setDiscount(double);				//设置品类折扣系数
	void setOffer(string);					//设置满减信息
	static vector<string> getCategories();	//获得分区信息
	static vector<double> getDiscountOfC();	//获得分区折扣信息
	static vector<string> getOfferOfC();	//获得分区满减信息
	static int getAmountOfC();				//获得分区总数量
	string getName();						//获得产品名称
	double getCurrentPrice();				//获得产品现价
	virtual string getCategory();			//获得产品类别
	double getExpressFee();					//获得快递费
	int getPurchaseQuantity();				//获得购买数量
	int getState();							//获得产品状态
	double getPayTime();					//获得付款时间
	double getDeliveryTime();				//获得发货时间
	double getDiscount();					//获得品类折扣系数
	string getOffer();						//获得满减信息
	void addSales(int);						//增加销量
	void decProductS(int);					//减少剩余量
	void priProduct();						//打印用户商品信息
	void priProductInList();				//打印列表中商品信息
	void priDetailedProduct();				//打印详细商品信息
	void priInfo();							//打印商品信息
	Product();
	~Product();
};

class Cosmetic :public Product {
private:
	string category;						//产品类别
public:
	void setCategory(string);				//设置产品类别
	string getCategory();					//获得产品类别
	Cosmetic();
	~Cosmetic();
};

class SportsOutdoor :public Product {
private:
	string category;						//产品类别
public:
	void setCategory(string);				//设置产品类别
	string getCategory();					//获得产品类别
	SportsOutdoor();
	~SportsOutdoor();
};

class ArticlesOfDailyUse :public Product {
private:
	string category;						//产品类别
public:
	void setCategory(string);				//设置产品类别
	string getCategory();					//获得产品类别
	ArticlesOfDailyUse();
	~ArticlesOfDailyUse();
};

class Women :public Product {
private:
	string category;						//产品类别
public:
	void setCategory(string);				//设置产品类别
	string getCategory();					//获得产品类别
	Women();
	~Women();
};

class Men :public Product {
private:
	string category;						//产品类别
public:
	void setCategory(string);				//设置产品类别
	string getCategory();					//获得产品类别
	Men();
	~Men();
};

class Food :public Product {
private:
	string category;						//产品类别
public:
	void setCategory(string);				//设置产品类别
	string getCategory();					//获得产品类别
	Food();
	~Food();
};

class User {
private:
	string username;						//会员名
	int amountOfSA;							//收货地址总数
	vector<Address> shippingAddress;		//收货地址
	Address defaultAddress;					//默认收货地址
	string phoneNumber;						//手机号码
	string password;						//密码
	int amountOfSC;							//购物车商品总数
	vector<Product*> shoppingCart;			//购物车
	int amountOfWFD;						//待发货商品总数
	vector<Product*> waitForDeliver;		//待发货
	int amountOfWFS;						//待收货商品总数
	vector<Product*> waitForSignature;		//待收货
public:
	static void readAnnotation();			//跳过注释信息
	static void writeAnnotation();			//向文件写入注释信息
	void read();							//读取信息
	void readAddress();						//读取地址
	void writeUser();						//向文件写入用户信息
	void setUsername(string);				//设置用户名
	void setAmountOfSA(int);				//设置收货地址总数
	void setPhoneNumber(string);			//设置手机号码
	void setPassword(string);				//设置密码
	void setAmountOfSC(int);				//设置购物车商品总数
	void setAmountOfWFD(int);				//设置待发货商品总数
	void setAmountOfWFS(int);				//设置待收货商品总数
	string getUsername();					//获得用户名
	vector<Address> getShippingAddress();	//获得收货地址
	void addShippingAddress();				//增添收货地址
	void modifyShippingAddress();			//修改收货地址
	void delShippingAddress();				//删除收货地址
	void priShippingAddress();				//打印收货地址
	Address getDA();						//获得默认地址
	void modifyDefaultAddress();			//修改默认地址
	string getPhoneNumber();				//获得手机号码
	string getPassword();					//获得密码
	void modifyPassword();					//修改密码
	void modifyName();						//修改用户名
	vector<Product*> getSC();				//获得购物车信息
	int getAmountOfSC();					//获得购物车商品数量
	void addShoppingCart(Product*);			//加入购物车
	int priShoppingCart();					//打印购物车信息
	void emptyShoppingCart();				//清空购物车
	void delPartOfSC();						//删除购物车商品
	void modifyPurchaseQuantity();			//修改购买数量
	double clearPartOfSC();					//部分结算
	double clearALLShoppingCart();			//全选结算
	vector<Product*> getWFD();				//获得待发货商品信息
	int getAmountOfWFD();					//获得待发货商品数量
	void priWFD();							//打印待发货商品信息
	void updateWFD();						//更新待发货商品（默认3天之内发货）
	vector<Product*> getWFS();				//获得待收货商品信息
	int getAmountOfWFS();					//获得待收货商品数量
	void priWFS();							//打印待收货商品时间
	void confirmSignature();				//确认收货
	void priUser();							//打印用户信息
	User();
	~User();
};

void splitString(string, vector<string>&, string);