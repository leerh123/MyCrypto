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
	bool IV_exist = false;//IV�Ƿ��Ѿ���ֵ
	bool nonce_exist = false;//nonce�Ƿ��Ѿ���ֵ
	string mode;
public:
	//keyΪ16�ֽ�
	//ECBģʽ��ʼ��
	AES(unsigned char* key_, string mode_);
	//CBC/CFBģʽ��ʼ��,key = unsigned char[16],IV = unsigned char[16]
	AES(unsigned char* key_, string mode_,unsigned char* IV_);
	//CTRģʽ��ʼ��,key=unsigned char[16],nonce = unsigned char[8]
	AES(unsigned char* key_, string mode_, unsigned char* nonce,bool ctr_mode);

	//ECBģʽ��ʼ��
	AES(string key_, string mode_);
	//CBC/CFBģʽ��ʼ��,key = unsigned char[16],IV = unsigned char[16]
	AES(string key_, string mode_, unsigned char* IV_);
	//CTRģʽ��ʼ��,key=unsigned char[16],nonce = unsigned char[8]
	AES(string key_, string mode_, unsigned char* nonce_, bool ctr_mode);

	//CBC/CFBģʽ��ʼ��,key = unsigned char[16],IV = unsigned char[16]
	AES(string key_, string mode_, string IV_);
	//CTRģʽ��ʼ��,key=unsigned char[16],nonce = unsigned char[8]
	AES(string key_, string mode_, string nonce_, bool ctr_mode);
	AES& operator=(AES a);
	~AES();
	vector<unsigned char> encrypt(string message);
	string decrypt(vector<unsigned char> message);
};

//һ�ֵ�AES���ܣ�message[16]Ϊ128���ص����ģ�key[16]Ϊ128������Կ����������C[16]
unsigned char* enc_1_block(unsigned char message[16], unsigned char key[16]);
//һ�ֵ�AES���ܣ�message[16]Ϊ128���ص����ģ�keyΪ128������Կ����������P[16]
unsigned char* dec_1_block(unsigned char message[16], unsigned char key[16]);

