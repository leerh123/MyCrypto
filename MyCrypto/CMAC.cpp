#include "CMAC.h"
#include"AES.h"
#include<iostream>
#include<sstream>
#include <iomanip>
using namespace std;
//将长度为len的字符串转为长度为len的unsigned char数组
inline static unsigned char* convert(string s, int len);
void generateSubKey(unsigned char* mK1, unsigned char* mK2, unsigned char* K);
void getCMAC(unsigned char* mac, unsigned char* msg, int len, unsigned char* K, unsigned char* mK1, unsigned char* mK2);

CMAC::CMAC(string key_, string msg_):msg(msg_) {
	key = key_;
	key.resize(16);

}
CMAC::CMAC(string key_ ) {
	key = key_;
	key.resize(16);
}
CMAC::~CMAC() {

}
CMAC& CMAC::operator=(CMAC a) {
	this->key = a.key;
	this->msg = a.msg;
	return *this;
}
void CMAC::update(string s) {
	msg += s;
}
string CMAC::hexdigest() {
    unsigned char* K = convert(key, 16);
    unsigned char* K1 = new unsigned char[16];
    unsigned char* K2 = new unsigned char[16];
    unsigned char* mac = new unsigned char[16];
    generateSubKey(K1, K2, K);
    unsigned char* msg_ = convert(msg, msg.size());
    getCMAC(mac, msg_, msg.size(), K, K1, K2);
    stringstream res;
    for (int i = 0; i < 16; ++i)
        res << hex << setfill('0') << setw(2) << int(mac[i]);
    delete[]K;
    delete[]K1;
    delete[]K2;
    delete[]mac;
    delete[]msg_;
    return res.str();

	
}
bool CMAC::hexverify(string hexdigest_) {
	return (hexdigest() == hexdigest_);
}

//将长度为len的字符串转为长度为len的unsigned char数组
inline static unsigned char* convert(string s, int len) {
    const char* charStr = s.c_str();
    unsigned char* ucArr = new unsigned char[len]();
    std::memcpy(ucArr, charStr, len);
    return ucArr;
}

const unsigned char const_Rb[16] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x87
};

const unsigned char const_Zero[16] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

void leftShiftOneBit(unsigned char* input_128, unsigned char* output_128)
{
    unsigned char overflow = 0x00;
    for (int i = 15; i >= 0; i--) {
        output_128[i] = input_128[i] << 1;
        output_128[i] |= overflow;
        overflow = (input_128[i] & 0x80) ? 0x01 : 0x00;
    }
    return;
}

void xor128(const unsigned char* a, const unsigned char* b, unsigned char* out)
{
    for (int i = 0; i < 16; i++) {
        out[i] = a[i] ^ b[i];
    }
}

void generateSubKey(unsigned char* mK1, unsigned char* mK2, unsigned char* K)
{
    //for output of AES-128 applied to 0^128
    unsigned char* L = new unsigned char[16];
    //0^128
    unsigned char Z[16] = { 0 };

    unsigned char temp[16];


    L = enc_1_block(Z, K);
    //If MSB(L)==0,then mK1=L<<1
    if ((L[0] & 0x80) == 0) {
        leftShiftOneBit(L, mK1);
    }
    //mK1=(L<<1)xor(Rb)
    else {
        leftShiftOneBit(L, temp);
        xor128(temp, const_Rb, mK1);
    }

    if ((mK1[0] & 0x80) == 0) {
        leftShiftOneBit(mK1, mK2);
    }
    else {
        leftShiftOneBit(mK1, temp);
        xor128(temp, const_Rb, mK2);
    }
    delete[] L;
    return;
}

void padding(unsigned char* lastb, unsigned char* pad, int length)
{
    for (int i = 0; i < 16; i++) {
        if (i < length) {
            pad[i] = lastb[i];
        }
        else if (i == length) {
            pad[i] = 0x80;
        }
        else {
            pad[i] = 0x00;
        }
    }
}

void getCMAC(unsigned char* mac, unsigned char* msg, int len, unsigned char* K,unsigned char* mK1,unsigned char* mK2)
{
    unsigned char* X = new unsigned char[16];
    unsigned char Y[16];
    unsigned char msgLast[16];

    unsigned char padded[16];

    //the number of blocks to be processed
    int n;
    //the number of unsigned chars of the last block
    int r;
    //denoting last block is complete or not
    bool isComplete;

    n = (len + 15) / 16;

    if (n == 0) {
        n = 1;
        isComplete = false;
    }
    else {
        if ((len % 16) == 0) {
            isComplete = true;
        }
        else {
            isComplete = false;
        }
    }

    if (isComplete) {
        xor128(&msg[16 * (n - 1)], mK1, msgLast);
    }
    else {
        padding(&msg[16 * (n - 1)], padded, len % 16);
        xor128(padded, mK2, msgLast);
    }

    for (int i = 0; i < 16; i++) {
        X[i] = 0;
    }
    for (int i = 0; i < n - 1; i++) {
        //Y=(X)xor(meg_i)
        xor128(X, &msg[16 * i], Y);
        //X=AES(key,Y)
        X = enc_1_block(Y,K);
    }

    xor128(X, msgLast, Y);
    X = enc_1_block(Y, K);

    for (int i = 0; i < 16; i++) {
        mac[i] = X[i];
    }
    delete[] X;

}