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
	string consignee;						//�ջ���
	string contactNumber;					//��ϵ�绰
	string detailedAddress;					//�����ַ
public:
	void readAddress();						//���ļ���ȡ��ַ��Ϣ
	void writeAddress();					//���ļ�д���ַ��Ϣ
	void setConsignee(string);				//�����ջ���
	void setContactNumber(string);			//������ϵ�绰
	void setDetailedAddress(string);		//���þ����ַ
	string getContactNumber();				//�����ϵ�绰
	void priAddress();						//��ӡ��ַ
	Address();
	~Address();
};

class Product {
private:
	static vector<string> categories;		//����
	static vector<double> discountOfC;		//�����ۿ�
	static vector<string> offerOfC;			//��������
	static int amountOfC;					//����������
	string name;							//��Ʒ����
	double originalPrice;					//��Ʒԭ��
	double currentPrice;					//��Ʒ�ּ�
	string category;						//��Ʒ���
	double expressFee;						//��ݷ�
	int sales;								//����
	string storeName;						//�������
	string storeAddress;					//��ҵ�ַ
	int productSurplus;						//��Ʒʣ����
	int purchaseQuantity;					//��������
	int state;								//��Ʒ״̬��1����������2�����ջ���
	double payTime;							//����ʱ��
	double deliveryTime;					//����ʱ��
	double discount;						//Ʒ���ۿ�ϵ��
	string offer;							//����
	string intro;							//����
public:
	static void readCategories();			//��ȡ������Ϣ
	static void readAnnotation();			//����ע����Ϣ
	static void writeAnnotation();			//���ļ�д��ע����Ϣ
	void readProduct();						//��ȡ��Ʒ��Ϣ
	void readForUser();						//��ȡ�û���Ʒ��Ϣ
	void writeProduct();					//���ļ�д���Ʒ��Ϣ
	void writeForUser();					//���ļ�д���û���Ʒ��Ϣ
	virtual void setCategory(string);		//���ò�Ʒ���
	void setPurchaseQuantity(int);			//���ù�������
	void setPurchaseQuantity(int, string);	//���ù�������
	void setState(int);						//���ò�Ʒ״̬
	void setPayTime(double);				//���ø���ʱ��
	void setDeliveryTime(double);			//���÷���ʱ��
	void setDiscount(double);				//����Ʒ���ۿ�ϵ��
	void setOffer(string);					//����������Ϣ
	static vector<string> getCategories();	//��÷�����Ϣ
	static vector<double> getDiscountOfC();	//��÷����ۿ���Ϣ
	static vector<string> getOfferOfC();	//��÷���������Ϣ
	static int getAmountOfC();				//��÷���������
	string getName();						//��ò�Ʒ����
	double getCurrentPrice();				//��ò�Ʒ�ּ�
	virtual string getCategory();			//��ò�Ʒ���
	double getExpressFee();					//��ÿ�ݷ�
	int getPurchaseQuantity();				//��ù�������
	int getState();							//��ò�Ʒ״̬
	double getPayTime();					//��ø���ʱ��
	double getDeliveryTime();				//��÷���ʱ��
	double getDiscount();					//���Ʒ���ۿ�ϵ��
	string getOffer();						//���������Ϣ
	void addSales(int);						//��������
	void decProductS(int);					//����ʣ����
	void priProduct();						//��ӡ�û���Ʒ��Ϣ
	void priProductInList();				//��ӡ�б�����Ʒ��Ϣ
	void priDetailedProduct();				//��ӡ��ϸ��Ʒ��Ϣ
	void priInfo();							//��ӡ��Ʒ��Ϣ
	Product();
	~Product();
};

class Cosmetic :public Product {
private:
	string category;						//��Ʒ���
public:
	void setCategory(string);				//���ò�Ʒ���
	string getCategory();					//��ò�Ʒ���
	Cosmetic();
	~Cosmetic();
};

class SportsOutdoor :public Product {
private:
	string category;						//��Ʒ���
public:
	void setCategory(string);				//���ò�Ʒ���
	string getCategory();					//��ò�Ʒ���
	SportsOutdoor();
	~SportsOutdoor();
};

class ArticlesOfDailyUse :public Product {
private:
	string category;						//��Ʒ���
public:
	void setCategory(string);				//���ò�Ʒ���
	string getCategory();					//��ò�Ʒ���
	ArticlesOfDailyUse();
	~ArticlesOfDailyUse();
};

class Women :public Product {
private:
	string category;						//��Ʒ���
public:
	void setCategory(string);				//���ò�Ʒ���
	string getCategory();					//��ò�Ʒ���
	Women();
	~Women();
};

class Men :public Product {
private:
	string category;						//��Ʒ���
public:
	void setCategory(string);				//���ò�Ʒ���
	string getCategory();					//��ò�Ʒ���
	Men();
	~Men();
};

class Food :public Product {
private:
	string category;						//��Ʒ���
public:
	void setCategory(string);				//���ò�Ʒ���
	string getCategory();					//��ò�Ʒ���
	Food();
	~Food();
};

class User {
private:
	string username;						//��Ա��
	int amountOfSA;							//�ջ���ַ����
	vector<Address> shippingAddress;		//�ջ���ַ
	Address defaultAddress;					//Ĭ���ջ���ַ
	string phoneNumber;						//�ֻ�����
	string password;						//����
	int amountOfSC;							//���ﳵ��Ʒ����
	vector<Product*> shoppingCart;			//���ﳵ
	int amountOfWFD;						//��������Ʒ����
	vector<Product*> waitForDeliver;		//������
	int amountOfWFS;						//���ջ���Ʒ����
	vector<Product*> waitForSignature;		//���ջ�
public:
	static void readAnnotation();			//����ע����Ϣ
	static void writeAnnotation();			//���ļ�д��ע����Ϣ
	void read();							//��ȡ��Ϣ
	void readAddress();						//��ȡ��ַ
	void writeUser();						//���ļ�д���û���Ϣ
	void setUsername(string);				//�����û���
	void setAmountOfSA(int);				//�����ջ���ַ����
	void setPhoneNumber(string);			//�����ֻ�����
	void setPassword(string);				//��������
	void setAmountOfSC(int);				//���ù��ﳵ��Ʒ����
	void setAmountOfWFD(int);				//���ô�������Ʒ����
	void setAmountOfWFS(int);				//���ô��ջ���Ʒ����
	string getUsername();					//����û���
	vector<Address> getShippingAddress();	//����ջ���ַ
	void addShippingAddress();				//�����ջ���ַ
	void modifyShippingAddress();			//�޸��ջ���ַ
	void delShippingAddress();				//ɾ���ջ���ַ
	void priShippingAddress();				//��ӡ�ջ���ַ
	Address getDA();						//���Ĭ�ϵ�ַ
	void modifyDefaultAddress();			//�޸�Ĭ�ϵ�ַ
	string getPhoneNumber();				//����ֻ�����
	string getPassword();					//�������
	void modifyPassword();					//�޸�����
	void modifyName();						//�޸��û���
	vector<Product*> getSC();				//��ù��ﳵ��Ϣ
	int getAmountOfSC();					//��ù��ﳵ��Ʒ����
	void addShoppingCart(Product*);			//���빺�ﳵ
	int priShoppingCart();					//��ӡ���ﳵ��Ϣ
	void emptyShoppingCart();				//��չ��ﳵ
	void delPartOfSC();						//ɾ�����ﳵ��Ʒ
	void modifyPurchaseQuantity();			//�޸Ĺ�������
	double clearPartOfSC();					//���ֽ���
	double clearALLShoppingCart();			//ȫѡ����
	vector<Product*> getWFD();				//��ô�������Ʒ��Ϣ
	int getAmountOfWFD();					//��ô�������Ʒ����
	void priWFD();							//��ӡ��������Ʒ��Ϣ
	void updateWFD();						//���´�������Ʒ��Ĭ��3��֮�ڷ�����
	vector<Product*> getWFS();				//��ô��ջ���Ʒ��Ϣ
	int getAmountOfWFS();					//��ô��ջ���Ʒ����
	void priWFS();							//��ӡ���ջ���Ʒʱ��
	void confirmSignature();				//ȷ���ջ�
	void priUser();							//��ӡ�û���Ϣ
	User();
	~User();
};

void splitString(string, vector<string>&, string);