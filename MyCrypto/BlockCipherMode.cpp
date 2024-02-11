#include "BlockCipherMode.h"
#include<sstream>
#include<string>
#include<iomanip>
#include<vector>
#include<string>
using namespace std;
//������Ϊlen���ַ���תΪ����Ϊlen��unsigned char����
inline static unsigned char* convert(string s,int len) {
	const char* charStr = s.c_str();  
	unsigned char* ucArr = new unsigned char[len]();
	std::memcpy(ucArr, charStr,len);
	return ucArr;
}

//�����ȳ���unsigned char�����������
inline static unsigned char* xor_byte (unsigned char* a, unsigned char* b, int len) {
	unsigned char* res = new unsigned char[len];
	for (int i = 0; i < len; ++i) {
		res[i] = a[i] ^ b[i];
	}
	return res;
}

//ECBģʽ����
vector<unsigned char> ECB_enc(string message, unsigned char* key,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key), 
	int len_1_group)
{

	int group = (message.size()+15) / len_1_group ;//���Է�Ϊgroup��
	unsigned char* message_1_group = new unsigned char[len_1_group];//ÿ�������
	unsigned char* res_1_group = new unsigned char[len_1_group];//ÿ����ܺ�Ľ��
	string sub_str;
	vector<unsigned char>tmp;
	vector<unsigned char>res;
	for (int i = 0; i < group; ++i) {
		//һ��len_1_group���ַ�
		sub_str = message.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str,len_1_group);
		//����
		res_1_group = enc(message_1_group, key);
		//������
		tmp = vector<unsigned char>(res_1_group, res_1_group + len_1_group);
		res.insert(res.end(), tmp.begin(), tmp.end());
	}
	delete[]message_1_group;
	delete[]res_1_group;
	return res;

}

//ECBģʽ����
string ECB_dec(vector<unsigned char> message, unsigned char* key,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key), 
	int len_1_group) 
{
	string s(message.begin(), message.end());
	int group = (s.size()+15) / len_1_group;//���Է�Ϊgroup��
	unsigned char* message_1_group = new unsigned char[len_1_group];//ÿ�������
	unsigned char* res_1_group = new unsigned char[len_1_group];//ÿ����ܺ�Ľ��
	string sub_str;
	vector<unsigned char>tmp;
	string res;
	for (int i = 0; i < group; ++i) {
		//һ��len_1_group���ַ�
		sub_str = s.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//����
		res_1_group = dec(message_1_group, key);
		//������
		res += string(res_1_group, res_1_group + len_1_group);
	}
	delete[]message_1_group;
	delete[]res_1_group;
	return res;
}

//CBCģʽ����
vector<unsigned char> CBC_enc(string message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {


	int group = (message.size() + 15) / len_1_group;//���Է�Ϊgroup��
	unsigned char* message_1_group = new unsigned char[len_1_group];//ÿ�������
	unsigned char* input_1_group = new unsigned char[len_1_group];//ÿ���м�����������
	unsigned char* res_1_group = new unsigned char[len_1_group];//ÿ����ܺ�Ľ��
	string sub_str;
	vector<unsigned char>tmp;
	vector<unsigned char>res;
	for (int i = 0; i < group; ++i) {
		//ÿ�������
		sub_str = message.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//ÿ�������������
		if (i == 0)
			input_1_group = xor_byte(IV, message_1_group, len_1_group);
		else
			input_1_group = xor_byte(res_1_group,message_1_group, len_1_group);
		//����
		res_1_group = enc(input_1_group, key);
		//������
		tmp = vector<unsigned char>(res_1_group, res_1_group + len_1_group);
		res.insert(res.end(), tmp.begin(), tmp.end());
	}
	delete[]message_1_group;
	delete[]input_1_group;
	delete[]res_1_group;
	return res;
}

//CBCģʽ����
string CBC_dec(vector<unsigned char> message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	string s(message.begin(), message.end());
	int group = (s.size() + 15) / len_1_group;//���Է�Ϊgroup��
	unsigned char* message_1_group = new unsigned char[len_1_group];//ÿ�������
	unsigned char* message_1_group_pre = new unsigned char[len_1_group];//��һ�������
	unsigned char* output_1_group = new unsigned char[len_1_group];//ÿ���н����������
	unsigned char* res_1_group = new unsigned char[len_1_group];//ÿ����ܺ�Ľ��
	string sub_str;
	vector<unsigned char>tmp;
	string res;
	for (int i = 0; i < group; ++i) {
		//ÿ�������
		sub_str = s.substr(i * len_1_group, len_1_group);
		message_1_group_pre = message_1_group;
		message_1_group = convert(sub_str, len_1_group);
		//���������
		output_1_group = dec(message_1_group, key);
		//ÿ�������
		if (i == 0)
			res_1_group = xor_byte(output_1_group, IV, len_1_group);
		else
			res_1_group = xor_byte(output_1_group, message_1_group_pre, len_1_group);
		//������
		res += string(res_1_group, res_1_group + len_1_group);
	}
	delete[]message_1_group;
	delete[]message_1_group_pre;
	delete[]output_1_group;
	delete[]res_1_group;
	return res;
}

//CTRģʽ����
vector<unsigned char> CTR_enc(string message, unsigned char* key, unsigned char* nonce,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	int group = (message.size() + 15) / len_1_group;//���Է�Ϊgroup��
	unsigned char* message_1_group = new unsigned char[len_1_group];//ÿ�������
	unsigned char* counter = new unsigned char[len_1_group];//������
	unsigned char* res_1_group = new unsigned char[len_1_group];//���������ܽ��
	string sub_str;
	vector<unsigned char>tmp;
	vector<unsigned char>res;
	//������ǰ���ֽ�Ϊnonce
	memcpy_s(counter, 8, nonce, 8);
	for (long long int i = 0; i < group; ++i) {
		//����
		sub_str = message.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//����������ֽ�
		counter[8] = (i >> 56) & 0xff;
		counter[9] = (i >> 48) & 0xff;
		counter[10] = (i >> 40) & 0xff;
		counter[11] = (i >> 32) & 0xff;
		counter[12] = (i >> 24) & 0xff;
		counter[13] = (i >> 16) & 0xff;
		counter[14] = (i >> 8) & 0xff;
		counter[15] = (i) & 0xff;
		//�Լ���������
		res_1_group = enc(counter, key);
		//������
		res_1_group = xor_byte(res_1_group, message_1_group, len_1_group);
		tmp = vector<unsigned char>(res_1_group, res_1_group + len_1_group);
		res.insert(res.end(), tmp.begin(), tmp.end());
	}
	delete[]message_1_group;
	delete[]counter;
	delete[]res_1_group;
	return res;
}

//CTRģʽ����
string CTR_dec(vector<unsigned char> message, unsigned char* key, unsigned char* nonce,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	string s(message.begin(), message.end());
	int group = (s.size() + 15) / len_1_group;//���Է�Ϊgroup��
	unsigned char* message_1_group = new unsigned char[len_1_group];//ÿ�������
	unsigned char* counter = new unsigned char[len_1_group];//������
	unsigned char* res_1_group = new unsigned char[len_1_group];//���������ܽ��
	string sub_str;
	vector<unsigned char>tmp;
	string res;
	//������ǰ���ֽ�Ϊnonce
	memcpy_s(counter, 8, nonce, 8);
	for (long long int i = 0; i < group; ++i) {
		//����
		sub_str = s.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//����������ֽ�
		counter[8] = (i >> 56) & 0xff;
		counter[9] = (i >> 48) & 0xff;
		counter[10] = (i >> 40) & 0xff;
		counter[11] = (i >> 32) & 0xff;
		counter[12] = (i >> 24) & 0xff;
		counter[13] = (i >> 16) & 0xff;
		counter[14] = (i >> 8) & 0xff;
		counter[15] = (i) & 0xff;
		//����
		res_1_group = enc(counter, key);
		//������
		res_1_group = xor_byte(res_1_group, message_1_group, len_1_group);
		res += string(res_1_group, res_1_group + len_1_group);
	}
	delete[]message_1_group;
	delete[]res_1_group;
	return res;
}

//CFBģʽ����
vector<unsigned char> CFB_enc(string message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	unsigned char* output_1_group = new unsigned char[len_1_group];//�����������
	unsigned char* input_1_group = new unsigned char[len_1_group];//������������
	unsigned char res_1_group;//ÿ����ܺ�Ľ��
	vector<unsigned char>res;
	for (size_t i = 0; i < message.size(); ++i) {
		if (i == 0) {
			output_1_group = enc(IV, key);
		}
		else {
			output_1_group = enc(input_1_group, key);
		}
		//ȡ���������������ֽ����������
		res_1_group = output_1_group[0] ^ message[i];
		//�����ֽ��뵱ǰ���Ϊ��һ�ּ��ܵ�����
		for (int j = 0; j < len_1_group - 1; ++j) {
			input_1_group[j] = output_1_group[j + 1];
		}
		input_1_group[len_1_group - 1] = res_1_group;
		//������
		res.push_back(res_1_group);
	}
	delete[]output_1_group;
	delete[]input_1_group;
	return res;
}
//CFBģʽ����
string CFB_dec(vector<unsigned char> message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	unsigned char* output_1_group = new unsigned char[len_1_group];//�����������
	unsigned char* input_1_group = new unsigned char[len_1_group];//������������
	unsigned char res_1_group;//ÿ����ܺ�Ľ��
	string res;
	for (size_t i = 0; i < message.size(); ++i) {
		if (i == 0) {
			output_1_group = enc(IV, key);
		}
		else {
			output_1_group = enc(input_1_group, key);
		}
		//ȡ���������������ֽ����������
		res_1_group = output_1_group[0] ^ message[i];
		//�����ֽ��뵱ǰ���Ϊ��һ�ּ��ܵ�����
		for (int j = 0; j < len_1_group - 1; ++j) {
			input_1_group[j] = output_1_group[j + 1];
		}
		input_1_group[len_1_group - 1] = message[i];
		//������
		res.push_back(res_1_group);
	}
	delete[]output_1_group;
	delete[]input_1_group;
	return res;
}

//OFBģʽ����
vector<unsigned char> OFB_enc(string message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	int group = (message.size() + 15) / len_1_group;//���Է�Ϊgroup��
	unsigned char* message_1_group = new unsigned char[len_1_group];//ÿ�������
	unsigned char* output_1_group = new unsigned char[len_1_group];//ÿ������������
	unsigned char* res_1_group = new unsigned char[len_1_group];//ÿ����ܺ�Ľ��
	string sub_str;
	vector<unsigned char>tmp;
	vector<unsigned char>res;
	for (int i = 0; i < group; ++i) {
		//����
		sub_str = message.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//������
		if (i == 0)
			output_1_group = enc(IV, key);
		else
			output_1_group = enc(output_1_group, key);
		
		//������
		res_1_group = xor_byte(message_1_group, output_1_group, len_1_group);
		tmp = vector<unsigned char>(res_1_group, res_1_group + len_1_group);
		res.insert(res.end(), tmp.begin(), tmp.end());
	}
	delete[]message_1_group;
	delete[]output_1_group;
	delete[]res_1_group;
	return res;
}

//OFBģʽ����
string OFB_dec(vector<unsigned char> message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	string s(message.begin(), message.end());
	int group = (s.size() + 15) / len_1_group;//���Է�Ϊgroup��
	unsigned char* message_1_group = new unsigned char[len_1_group];//ÿ�������
	unsigned char* output_1_group = new unsigned char[len_1_group];//ÿ������������
	unsigned char* res_1_group = new unsigned char[len_1_group];//ÿ����ܺ�Ľ��
	string sub_str;
	vector<unsigned char>tmp;
	string res;
	for (int i = 0; i < group; ++i) {
		//����
		sub_str = s.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//������
		if (i == 0)
			output_1_group = enc(IV, key);
		else
			output_1_group = enc(output_1_group, key);

		//������
		res_1_group = xor_byte(message_1_group, output_1_group, len_1_group);
		//������
		res += string(res_1_group, res_1_group + len_1_group);
	}
	delete[]message_1_group;
	delete[]output_1_group;
	delete[]res_1_group;

	return res;
}