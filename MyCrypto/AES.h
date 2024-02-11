#pragma once
#include<string>
#include<vector>
using std::string;
using std::vector;
class AES
{
private:
	unsigned char* key = nullptr;
	unsigned char* IV = nullptr;
	unsigned char* nonce = nullptr;
	bool IV_exist = false;//IV是否已经赋值
	bool nonce_exist = false;//nonce是否已经赋值
	string mode;
public:
	//key为16字节
	//ECB模式初始化
	AES(unsigned char* key_, string mode_);
	//CBC/CFB模式初始化,key = unsigned char[16],IV = unsigned char[16]
	AES(unsigned char* key_, string mode_,unsigned char* IV_);
	//CTR模式初始化,key=unsigned char[16],nonce = unsigned char[8]
	AES(unsigned char* key_, string mode_, unsigned char* nonce,bool ctr_mode);

	//ECB模式初始化
	AES(string key_, string mode_);
	//CBC/CFB模式初始化,key = unsigned char[16],IV = unsigned char[16]
	AES(string key_, string mode_, unsigned char* IV_);
	//CTR模式初始化,key=unsigned char[16],nonce = unsigned char[8]
	AES(string key_, string mode_, unsigned char* nonce_, bool ctr_mode);

	//CBC/CFB模式初始化,key = unsigned char[16],IV = unsigned char[16]
	AES(string key_, string mode_, string IV_);
	//CTR模式初始化,key=unsigned char[16],nonce = unsigned char[8]
	AES(string key_, string mode_, string nonce_, bool ctr_mode);
	AES& operator=(AES a);
	~AES();
	vector<unsigned char> encrypt(string message);
	string decrypt(vector<unsigned char> message);
};

//一轮的AES加密：message[16]为128比特的明文，key[16]为128比特密钥，返回密文C[16]
unsigned char* enc_1_block(unsigned char message[16], unsigned char key[16]);
//一轮的AES解密：message[16]为128比特的密文，key为128比特密钥，返回明文P[16]
unsigned char* dec_1_block(unsigned char message[16], unsigned char key[16]);

