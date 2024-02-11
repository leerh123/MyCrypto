#pragma once
#include<string>
#include<vector>
using std::string;
using std::vector;
vector<unsigned char> ECB_enc(string message, unsigned char* key,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);
string ECB_dec(vector<unsigned char> message, unsigned char* key,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);
vector<unsigned char> CBC_enc(string message, unsigned char* key,unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);
string CBC_dec(vector<unsigned char> message, unsigned char* key,unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);
//仅适用于AES
vector<unsigned char> CTR_enc(string message, unsigned char* key, unsigned char* nonce,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);
//仅适用于AES
string CTR_dec(vector<unsigned char> message, unsigned char* key, unsigned char* nonce,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);

vector<unsigned char> CFB_enc(string message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);
string CFB_dec(vector<unsigned char> message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);

vector<unsigned char> OFB_enc(string message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);
string OFB_dec(vector<unsigned char> message, unsigned char* key, unsigned char* IV,
	unsigned char* (*enc)(unsigned char* message, unsigned char* key), unsigned char* (*dec)(unsigned char* message, unsigned char* key),
	int len_1_group);