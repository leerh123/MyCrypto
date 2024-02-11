#include "BlockCipherMode.h"
#include<sstream>
#include<string>
#include<iomanip>
#include<vector>
#include<string>
using namespace std;
//将长度为len的字符串转为长度为len的unsigned char数组
inline static unsigned char* convert(string s,int len) {
	const char* charStr = s.c_str();  
	unsigned char* ucArr = new unsigned char[len]();
	std::memcpy(ucArr, charStr,len);
	return ucArr;
}

//两个等长的unsigned char数组逐项异或
inline static unsigned char* xor_byte (unsigned char* a, unsigned char* b, int len) {
	unsigned char* res = new unsigned char[len];
	for (int i = 0; i < len; ++i) {
		res[i] = a[i] ^ b[i];
	}
	return res;
}

//ECB模式加密
vector<unsigned char> ECB_enc(string message, unsigned char* key,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key), 
	int len_1_group)
{

	int group = (message.size()+15) / len_1_group ;//可以分为group组
	unsigned char* message_1_group = new unsigned char[len_1_group];//每组的明文
	unsigned char* res_1_group = new unsigned char[len_1_group];//每组加密后的结果
	string sub_str;
	vector<unsigned char>tmp;
	vector<unsigned char>res;
	for (int i = 0; i < group; ++i) {
		//一组len_1_group个字符
		sub_str = message.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str,len_1_group);
		//加密
		res_1_group = enc(message_1_group, key);
		//输出结果
		tmp = vector<unsigned char>(res_1_group, res_1_group + len_1_group);
		res.insert(res.end(), tmp.begin(), tmp.end());
	}
	delete[]message_1_group;
	delete[]res_1_group;
	return res;

}

//ECB模式解密
string ECB_dec(vector<unsigned char> message, unsigned char* key,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key), 
	int len_1_group) 
{
	string s(message.begin(), message.end());
	int group = (s.size()+15) / len_1_group;//可以分为group组
	unsigned char* message_1_group = new unsigned char[len_1_group];//每组的密文
	unsigned char* res_1_group = new unsigned char[len_1_group];//每组解密后的结果
	string sub_str;
	vector<unsigned char>tmp;
	string res;
	for (int i = 0; i < group; ++i) {
		//一组len_1_group个字符
		sub_str = s.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//解密
		res_1_group = dec(message_1_group, key);
		//输出结果
		res += string(res_1_group, res_1_group + len_1_group);
	}
	delete[]message_1_group;
	delete[]res_1_group;
	return res;
}

//CBC模式加密
vector<unsigned char> CBC_enc(string message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {


	int group = (message.size() + 15) / len_1_group;//可以分为group组
	unsigned char* message_1_group = new unsigned char[len_1_group];//每组的明文
	unsigned char* input_1_group = new unsigned char[len_1_group];//每组中加密器的输入
	unsigned char* res_1_group = new unsigned char[len_1_group];//每组加密后的结果
	string sub_str;
	vector<unsigned char>tmp;
	vector<unsigned char>res;
	for (int i = 0; i < group; ++i) {
		//每组的明文
		sub_str = message.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//每组加密器的输入
		if (i == 0)
			input_1_group = xor_byte(IV, message_1_group, len_1_group);
		else
			input_1_group = xor_byte(res_1_group,message_1_group, len_1_group);
		//加密
		res_1_group = enc(input_1_group, key);
		//输出结果
		tmp = vector<unsigned char>(res_1_group, res_1_group + len_1_group);
		res.insert(res.end(), tmp.begin(), tmp.end());
	}
	delete[]message_1_group;
	delete[]input_1_group;
	delete[]res_1_group;
	return res;
}

//CBC模式解密
string CBC_dec(vector<unsigned char> message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	string s(message.begin(), message.end());
	int group = (s.size() + 15) / len_1_group;//可以分为group组
	unsigned char* message_1_group = new unsigned char[len_1_group];//每组的密文
	unsigned char* message_1_group_pre = new unsigned char[len_1_group];//上一组的密文
	unsigned char* output_1_group = new unsigned char[len_1_group];//每组中解密器的输出
	unsigned char* res_1_group = new unsigned char[len_1_group];//每组解密后的结果
	string sub_str;
	vector<unsigned char>tmp;
	string res;
	for (int i = 0; i < group; ++i) {
		//每组的密文
		sub_str = s.substr(i * len_1_group, len_1_group);
		message_1_group_pre = message_1_group;
		message_1_group = convert(sub_str, len_1_group);
		//解密器输出
		output_1_group = dec(message_1_group, key);
		//每组的明文
		if (i == 0)
			res_1_group = xor_byte(output_1_group, IV, len_1_group);
		else
			res_1_group = xor_byte(output_1_group, message_1_group_pre, len_1_group);
		//输出结果
		res += string(res_1_group, res_1_group + len_1_group);
	}
	delete[]message_1_group;
	delete[]message_1_group_pre;
	delete[]output_1_group;
	delete[]res_1_group;
	return res;
}

//CTR模式加密
vector<unsigned char> CTR_enc(string message, unsigned char* key, unsigned char* nonce,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	int group = (message.size() + 15) / len_1_group;//可以分为group组
	unsigned char* message_1_group = new unsigned char[len_1_group];//每组的明文
	unsigned char* counter = new unsigned char[len_1_group];//计数器
	unsigned char* res_1_group = new unsigned char[len_1_group];//计数器加密结果
	string sub_str;
	vector<unsigned char>tmp;
	vector<unsigned char>res;
	//计数器前八字节为nonce
	memcpy_s(counter, 8, nonce, 8);
	for (long long int i = 0; i < group; ++i) {
		//明文
		sub_str = message.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//计数器后八字节
		counter[8] = (i >> 56) & 0xff;
		counter[9] = (i >> 48) & 0xff;
		counter[10] = (i >> 40) & 0xff;
		counter[11] = (i >> 32) & 0xff;
		counter[12] = (i >> 24) & 0xff;
		counter[13] = (i >> 16) & 0xff;
		counter[14] = (i >> 8) & 0xff;
		counter[15] = (i) & 0xff;
		//对计数器加密
		res_1_group = enc(counter, key);
		//输出结果
		res_1_group = xor_byte(res_1_group, message_1_group, len_1_group);
		tmp = vector<unsigned char>(res_1_group, res_1_group + len_1_group);
		res.insert(res.end(), tmp.begin(), tmp.end());
	}
	delete[]message_1_group;
	delete[]counter;
	delete[]res_1_group;
	return res;
}

//CTR模式解密
string CTR_dec(vector<unsigned char> message, unsigned char* key, unsigned char* nonce,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	string s(message.begin(), message.end());
	int group = (s.size() + 15) / len_1_group;//可以分为group组
	unsigned char* message_1_group = new unsigned char[len_1_group];//每组的密文
	unsigned char* counter = new unsigned char[len_1_group];//计数器
	unsigned char* res_1_group = new unsigned char[len_1_group];//计数器加密结果
	string sub_str;
	vector<unsigned char>tmp;
	string res;
	//计数器前八字节为nonce
	memcpy_s(counter, 8, nonce, 8);
	for (long long int i = 0; i < group; ++i) {
		//密文
		sub_str = s.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//计数器后八字节
		counter[8] = (i >> 56) & 0xff;
		counter[9] = (i >> 48) & 0xff;
		counter[10] = (i >> 40) & 0xff;
		counter[11] = (i >> 32) & 0xff;
		counter[12] = (i >> 24) & 0xff;
		counter[13] = (i >> 16) & 0xff;
		counter[14] = (i >> 8) & 0xff;
		counter[15] = (i) & 0xff;
		//加密
		res_1_group = enc(counter, key);
		//输出结果
		res_1_group = xor_byte(res_1_group, message_1_group, len_1_group);
		res += string(res_1_group, res_1_group + len_1_group);
	}
	delete[]message_1_group;
	delete[]res_1_group;
	return res;
}

//CFB模式加密
vector<unsigned char> CFB_enc(string message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	unsigned char* output_1_group = new unsigned char[len_1_group];//加密器的输出
	unsigned char* input_1_group = new unsigned char[len_1_group];//加密器的输入
	unsigned char res_1_group;//每组加密后的结果
	vector<unsigned char>res;
	for (size_t i = 0; i < message.size(); ++i) {
		if (i == 0) {
			output_1_group = enc(IV, key);
		}
		else {
			output_1_group = enc(input_1_group, key);
		}
		//取加密器输出的最高字节与明文异或
		res_1_group = output_1_group[0] ^ message[i];
		//其余字节与当前结果为下一轮加密的输入
		for (int j = 0; j < len_1_group - 1; ++j) {
			input_1_group[j] = output_1_group[j + 1];
		}
		input_1_group[len_1_group - 1] = res_1_group;
		//输出结果
		res.push_back(res_1_group);
	}
	delete[]output_1_group;
	delete[]input_1_group;
	return res;
}
//CFB模式解密
string CFB_dec(vector<unsigned char> message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	unsigned char* output_1_group = new unsigned char[len_1_group];//加密器的输出
	unsigned char* input_1_group = new unsigned char[len_1_group];//加密器的输入
	unsigned char res_1_group;//每组解密后的结果
	string res;
	for (size_t i = 0; i < message.size(); ++i) {
		if (i == 0) {
			output_1_group = enc(IV, key);
		}
		else {
			output_1_group = enc(input_1_group, key);
		}
		//取加密器输出的最高字节与明文异或
		res_1_group = output_1_group[0] ^ message[i];
		//其余字节与当前结果为下一轮加密的输入
		for (int j = 0; j < len_1_group - 1; ++j) {
			input_1_group[j] = output_1_group[j + 1];
		}
		input_1_group[len_1_group - 1] = message[i];
		//输出结果
		res.push_back(res_1_group);
	}
	delete[]output_1_group;
	delete[]input_1_group;
	return res;
}

//OFB模式加密
vector<unsigned char> OFB_enc(string message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	int group = (message.size() + 15) / len_1_group;//可以分为group组
	unsigned char* message_1_group = new unsigned char[len_1_group];//每组的明文
	unsigned char* output_1_group = new unsigned char[len_1_group];//每组加密器的输出
	unsigned char* res_1_group = new unsigned char[len_1_group];//每组加密后的结果
	string sub_str;
	vector<unsigned char>tmp;
	vector<unsigned char>res;
	for (int i = 0; i < group; ++i) {
		//明文
		sub_str = message.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//加密器
		if (i == 0)
			output_1_group = enc(IV, key);
		else
			output_1_group = enc(output_1_group, key);
		
		//输出结果
		res_1_group = xor_byte(message_1_group, output_1_group, len_1_group);
		tmp = vector<unsigned char>(res_1_group, res_1_group + len_1_group);
		res.insert(res.end(), tmp.begin(), tmp.end());
	}
	delete[]message_1_group;
	delete[]output_1_group;
	delete[]res_1_group;
	return res;
}

//OFB模式解密
string OFB_dec(vector<unsigned char> message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group) {

	string s(message.begin(), message.end());
	int group = (s.size() + 15) / len_1_group;//可以分为group组
	unsigned char* message_1_group = new unsigned char[len_1_group];//每组的密文
	unsigned char* output_1_group = new unsigned char[len_1_group];//每组加密器的输出
	unsigned char* res_1_group = new unsigned char[len_1_group];//每组解密后的结果
	string sub_str;
	vector<unsigned char>tmp;
	string res;
	for (int i = 0; i < group; ++i) {
		//密文
		sub_str = s.substr(i * len_1_group, len_1_group);
		message_1_group = convert(sub_str, len_1_group);
		//加密器
		if (i == 0)
			output_1_group = enc(IV, key);
		else
			output_1_group = enc(output_1_group, key);

		//输出结果
		res_1_group = xor_byte(message_1_group, output_1_group, len_1_group);
		//输出结果
		res += string(res_1_group, res_1_group + len_1_group);
	}
	delete[]message_1_group;
	delete[]output_1_group;
	delete[]res_1_group;

	return res;
}