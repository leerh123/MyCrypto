#include "AES.h"
#include"BlockCipherMode.h"
#include<string>
#include<vector>
#include<cstring>
#include<random>
using namespace std;
AES::AES(unsigned char* key_, string mode_) :mode(mode_) {
    key = new unsigned char[16];
    memcpy_s(key, 16, key_, 16);

    IV = new unsigned char[16];
    IV_exist = false;

    nonce = new unsigned char[8];
    nonce_exist = false;
}

AES::AES(string key_, string mode_) :mode(mode_) {
    key_.resize(16);
    key = new unsigned char[16];
    memcpy_s(key, 16, key_.c_str(), 16);

    IV = new unsigned char[16];
    IV_exist = false;

    nonce = new unsigned char[8];
    nonce_exist = false;
}

AES::AES(unsigned char* key_, string mode_, unsigned char* IV_):mode(mode_) {
    key = new unsigned char[16];
    memcpy_s(key, 16, key_, 16);

    IV = new unsigned char[16];
    memcpy_s(IV, 16, IV_, 16);
    IV_exist = true;

    nonce = new unsigned char[8];
    nonce_exist = false;
}

AES::AES(string key_, string mode_, unsigned char* IV_) :mode(mode_) {
    key_.resize(16);
    key = new unsigned char[16];
    memcpy_s(key, 16, key_.c_str(), 16);

    IV = new unsigned char[16];
    memcpy_s(IV, 16, IV_, 16);
    IV_exist = true;

    nonce = new unsigned char[8];
    nonce_exist = false;
}

AES::AES(string key_, string mode_, string IV_) :mode(mode_) {
    key_.resize(16);
    key = new unsigned char[16];
    memcpy_s(key, 16, key_.c_str(), 16);

    IV_.resize(16);
    IV = new unsigned char[16];
    memcpy_s(IV, 16, IV_.c_str(), 16);
    IV_exist = true;

    nonce = new unsigned char[8];
    nonce_exist = false;
}

//CTR模式初始化,key=unsigned char[16],nonce = unsigned char[8]
AES::AES(unsigned char* key_, string mode_, unsigned char* nonce_, bool ctr_mode):mode(mode_) {
    key = new unsigned char[16];
    memcpy_s(key, 16, key_, 16);

    IV = new unsigned char[16];
    IV_exist = false;
    
    nonce = new unsigned char[8];
    memcpy_s(nonce, 8, nonce_, 8);
    nonce_exist = true;


}

AES::AES(string key_, string mode_, unsigned char* nonce_, bool ctr_mode) :mode(mode_) {
    key_.resize(16);
    key = new unsigned char[16];
    memcpy_s(key, 16, key_.c_str(), 16);

    IV = new unsigned char[16];
    IV_exist = false;

    nonce = new unsigned char[8];
    memcpy_s(nonce, 8, nonce_, 8);
    nonce_exist = true;
}

AES::AES(string key_, string mode_, string nonce_, bool ctr_mode) {
    key_.resize(16);
    key = new unsigned char[16];
    memcpy_s(key, 16, key_.c_str(), 16);

    IV = new unsigned char[16];
    IV_exist = false;

    nonce_.resize(8);
    nonce = new unsigned char[8];
    memcpy_s(nonce, 8, nonce_.c_str(), 8);
    nonce_exist = true;

}

AES& AES::operator=(AES a) {
    this->nonce_exist = a.nonce_exist;
	this->IV_exist = a.IV_exist;
	this->mode = a.mode;
    memcpy_s(a.key, 16, this->key, 16);
    memcpy_s(a.IV, 16, this->IV, 16);
    memcpy_s(a.nonce, 8, this->nonce, 8);
	return *this;
}

AES::~AES() {
    delete[] key;
    delete[] IV;
    delete[] nonce;
}
vector<unsigned char> AES::encrypt(string message) {
    vector<unsigned char> res;
    if (mode == "ECB") 
        res = ECB_enc(message, key, enc_1_block, dec_1_block, 16);
    else if(mode == "CBC") {
        //不存在IV就随机生成一个IV
        if (IV_exist == false) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<unsigned int> dis(0, 255);
            for (int i = 0; i < 16; ++i) {
                IV[i] = static_cast<unsigned char>(dis(gen));
            }
        }
        res = CBC_enc(message, key, IV, enc_1_block, dec_1_block, 16);
    }
    else if (mode == "CTR") {
        //不存在nonce就随机生成一个nonce
        if (nonce_exist = false) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<unsigned int> dis(0, 255);
            for (int i = 0; i < 8; ++i) {
                IV[i] = static_cast<unsigned char>(dis(gen));
            }
        }
        res = CTR_enc(message, key, nonce, enc_1_block, dec_1_block, 16);
    }
    else if (mode == "CFB") {
        //不存在IV就随机生成一个IV
        if (IV_exist == false) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<unsigned int> dis(0, 255);
            for (int i = 0; i < 16; ++i) {
                IV[i] = static_cast<unsigned char>(dis(gen));
            }
        }
        res = CFB_enc(message, key, IV, enc_1_block, dec_1_block, 16);
    }
    else if (mode == "OFB") {
        //不存在IV就随机生成一个IV
        if (IV_exist == false) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<unsigned int> dis(0, 255);
            for (int i = 0; i < 16; ++i) {
                IV[i] = static_cast<unsigned char>(dis(gen));
            }
        }

        res = OFB_enc(message, key, IV, enc_1_block, dec_1_block, 16);
    }
    else{}

    return res;
}

string AES::decrypt(vector<unsigned char> message) {
    string res;
    if (mode == "ECB")
        res = ECB_dec(message, key, enc_1_block, dec_1_block, 16);
    else if (mode == "CBC") 
        res = CBC_dec(message, key, IV, enc_1_block, dec_1_block, 16);
    else if (mode == "CTR")
        res = CTR_dec(message, key, nonce, enc_1_block, dec_1_block, 16);
    else if(mode == "CFB")
        res = CFB_dec(message, key, IV, enc_1_block, dec_1_block, 16);
    else if(mode == "OFB")
        res = OFB_dec(message, key, IV, enc_1_block, dec_1_block, 16);
    else {}
    return res;
}

#include <iostream>
#include<sstream>
#include <iomanip>
using namespace std;
//********常量********
// S盒
unsigned char S[256] =
{
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

//逆S盒
unsigned char inv_S[256] =
{
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

//列混淆时用到的正矩阵
extern unsigned char positive_matrix[4][4] =
{
    0x02, 0x03, 0x01, 0x01,
    0x01, 0x02, 0x03, 0x01,
    0x01, 0x01, 0x02, 0x03,
    0x03, 0x01, 0x01, 0x02
};

//逆列混淆时用到的逆正矩阵
extern unsigned char inv_positive_matrix[4][4] =
{
    0x0e, 0x0b, 0x0d, 0x09,
    0x09, 0x0e, 0x0b, 0x0d,
    0x0d, 0x09, 0x0e, 0x0b,
    0x0b, 0x0d, 0x09, 0x0e
};

//********函数声明********
// 密钥扩展函数
unsigned int* KeyExpansion(unsigned char* init_key);

//将128比特转换为状态矩阵
void array_to_mat(unsigned char p[], unsigned char state_mat[][4]);

//逆操作
void mat_to_array(unsigned char state_mat[][4], unsigned char c[]);

//轮密钥加，cnt标记这是第几轮循环，正确
void AddRoundKey(unsigned int W[44], unsigned char state_mat[][4], int cnt);

//行位移
void ShiftRows(unsigned char state_mat[][4]);

//逆行位移
void Inv_ShiftRows(unsigned char state_mat[][4]);

//S盒
unsigned char SubBytes(unsigned char input);

//逆S盒
unsigned char Inv_SubBytes(unsigned char input);

//列混合
void MixColumns(unsigned char state_mat[][4]);

//逆列混合
void Inv_MixColumns(unsigned char state_mat[][4]);

//********函数实现********
//一轮的AES加密：message[16]为128比特的明文，key[16]为128比特密钥，返回密文C[16]
unsigned char* enc_1_block(unsigned char message[16], unsigned char key[16]) {
    //密钥扩展
    unsigned int* full_key = KeyExpansion(key);
    //首轮
    unsigned char state_mat[4][4];
    array_to_mat(message, state_mat);
    AddRoundKey(full_key, state_mat, 0);
    //十轮
    for (int i = 1; i <= 10; i++)
    {
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                state_mat[row][col] = SubBytes(state_mat[row][col]);
            }
        }
        ShiftRows(state_mat);
        if (i != 10)
        {
            MixColumns(state_mat);
        }
        AddRoundKey(full_key, state_mat, i);
    }
    unsigned char* C = new unsigned char[16];
    mat_to_array(state_mat, C);
    delete[]full_key;
    return C;
}

//一轮的AES解密：message[16]为128比特的密文，key为128比特密钥，返回明文P[16]
unsigned char* dec_1_block(unsigned char message[16], unsigned char key[16]) {
    //密钥扩展
    unsigned int* full_key = KeyExpansion(key);
    //首轮
    unsigned char state_mat[4][4];
    array_to_mat(message, state_mat);
    AddRoundKey(full_key, state_mat, 10);
    for (int i = 9; i >= 0; i--)
    {
        Inv_ShiftRows(state_mat);
        for (int row = 0; row < 4; row++) //字节替换
        {
            for (int col = 0; col < 4; col++)
            {
                state_mat[row][col] = Inv_SubBytes(state_mat[row][col]);
            }
        }
        AddRoundKey(full_key, state_mat, i);
        if (i != 0)
        {
            Inv_MixColumns(state_mat);
        }
    }
    unsigned char* P = new unsigned char[16];
    mat_to_array(state_mat, P);
    delete[]full_key;
    return P;
}

//将1个32位的密钥，转换为4个8位密钥,key8[0]为原来key32的高位
void key32_to_key8(unsigned int key32, unsigned char* key8)
{
    unsigned int mark = 0xff;
    int left_bit;
    for (int i = 0; i < 4; i++)
    {
        left_bit = (3 - i) * 8;
        key8[i] = ((mark << left_bit) & key32) >> left_bit;
    }
}

//将4个8位的密钥，转换为1个32位密钥
unsigned int key8_to_key32(unsigned char* key8)
{
    unsigned int temp_key = 0;
    for (int i = 0; i < 4; i++)
    {
        temp_key ^= ((unsigned int)key8[i] << ((3 - i) * 8));
        if (i == 3)
        {
            return temp_key;
        }
    }

}

//S盒,前4比特为行标，后4比特为列标
unsigned char SubBytes(unsigned char input)
{
    unsigned char pre = 0xf0;
    unsigned char suf = 0x0f;
    unsigned int row = (input & pre) >> 4, col = input & suf;
    return S[row * 16UL + col];
}


//密钥扩展时的T函数，cnt代表轮数
unsigned int T(unsigned int input, int cnt)
{
    // AES-128轮常量
    static const unsigned int rcon[10] = {
        0x01000000UL, 0x02000000UL, 0x04000000UL, 0x08000000UL, 0x10000000UL,
        0x20000000UL, 0x40000000UL, 0x80000000UL, 0x1B000000UL, 0x36000000UL
    };
    unsigned char key8[4];
    key32_to_key8(input, key8);
    unsigned char temp[4];
    temp[0] = SubBytes(key8[1]);
    temp[1] = SubBytes(key8[2]);
    temp[2] = SubBytes(key8[3]);
    temp[3] = SubBytes(key8[0]);
    unsigned int ans = key8_to_key32(temp);
    ans = ans ^ rcon[cnt - 1];
    return ans;
}

//密钥扩展函数
unsigned int* KeyExpansion(unsigned char* init_key)
{
    unsigned int* W = new unsigned int[44];
    unsigned int temp_key = 0;
    for (int i = 0; i < 16; i++) // 对输入的8位初始密钥，转换到32位的W[0],W[1],W[2],W[3]中
    {
        temp_key ^= ((unsigned int)init_key[i] << ((3 - (i % 4)) * 8));
        if (i % 4 == 3)
        {
            W[i / 4] = temp_key;
            temp_key = 0;
        }
    }
    int cnt;
    for (int i = 1; i <= 10; i++) //10轮密钥扩展
    {
        cnt = i * 4; //cnt来当做更新W的下标
        while (1)
        {
            if (cnt % 4 != 0)
            {
                W[cnt] = W[cnt - 1] ^ W[cnt - 4];
            }
            else
            {
                W[cnt] = W[cnt - 4] ^ T(W[cnt - 1], i);
            }
            cnt++;
            if (cnt % 4 == 0)
            {
                break;
            }
        }
    }
    return W;
}

//将128比特转换为状态矩阵
void array_to_mat(unsigned char p[], unsigned char state_mat[][4])
{
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            state_mat[row][col] = p[col * 4 + row];
        }
    }
}

//轮密钥加，cnt标记这是第几轮循环，正确
void AddRoundKey(unsigned int W[44], unsigned char state_mat[][4], int cnt)
{
    unsigned char key8[4];
    for (int col = 0; col < 4; col++)
    {
        key32_to_key8(W[4 * cnt + col], key8);
        for (int row = 0; row < 4; row++)
        {
            state_mat[row][col] ^= key8[row];
        }
    }
}

//行位移
void ShiftRows(unsigned char state_mat[][4])
{
    unsigned char temp_row[4];
    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp_row[j] = state_mat[i][(j + 4 + i) % 4];
        }
        for (int j = 0; j < 4; j++)
        {
            state_mat[i][j] = temp_row[j];
        }
    }
}

//有限域上的乘法
unsigned char multi_finite_field(unsigned char a, unsigned char b)
{
    unsigned char ans = 0, v;
    for (int counter = 0; counter < 8; counter++)
    {
        if ((b & 0x01) != 0)
        {
            ans ^= a;
        }
        v = a >> 7;
        a <<= 1;
        if (v != 0)
        {
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return ans;

}

//列混合
void MixColumns(unsigned char state_mat[][4])
{
    unsigned char ans_mat[4][4];
    memset(ans_mat, 0, sizeof(ans_mat));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                ans_mat[i][j] ^= multi_finite_field(positive_matrix[i][k], state_mat[k][j]);
            }
        }
    }
    memcpy(state_mat, ans_mat, sizeof(ans_mat));
}

//将状态矩阵转换为128密文
void mat_to_array(unsigned char state_mat[][4], unsigned char c[])
{
    int cnt = 0;
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            c[cnt++] = state_mat[row][col];
        }
    }
}

//逆字节替换
unsigned char Inv_SubBytes(unsigned char input)
{
    unsigned char pre = 0xf0;
    unsigned char suf = 0x0f;
    unsigned int row = (input & pre) >> 4, col = input & suf;
    return inv_S[row * 16UL + col];
}

//逆行位移
void Inv_ShiftRows(unsigned char state_mat[][4])
{
    unsigned char temp_row[4];
    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp_row[(j + 4 + i) % 4] = state_mat[i][j];  //右移i位
        }
        for (int j = 0; j < 4; j++)
        {
            state_mat[i][j] = temp_row[j];
        }
    }
}

//逆列混合
void Inv_MixColumns(unsigned char state_mat[][4])
{
    unsigned char ans_mat[4][4];
    memset(ans_mat, 0, sizeof(ans_mat));
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                ans_mat[i][j] ^= multi_finite_field(inv_positive_matrix[i][k], state_mat[k][j]);
            }
        }
    }
    memcpy(state_mat, ans_mat, sizeof(ans_mat));
}
