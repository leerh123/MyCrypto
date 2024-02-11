#include "SHA1.h"
string sha1_hex(string text);
SHA1::SHA1() :HashSuperClass() {}
SHA1::SHA1(string s) :HashSuperClass(s) {}
string SHA1::hexdigest() {
	return sha1_hex(text);
}

#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>
using namespace std;
//m为原始明文，处理后的明文长度为512*n bit，处理后的结果在output中保存
static void padding(vector<unsigned char>& output, int& n, string m) {
    n = (m.size() + 8) / 64 + 1;
    output.resize(n * 64);
    int i = 0;
    //原始明文消息
    for (; i < m.size(); i++) {
        output[i] = m[i];
    }
    //填充10...0
    output[i++] = 0x80;
    while (i < output.size() - 8) {
        output[i] = 0;
        i++;
    }
    //填充长度
    long long int len = m.size() * 8;
    for (i = output.size() - 1; i >= output.size() - 8; i--) {
        output[i] = len % 256;
        len /= 256;
    }
}
//f(t,B,C,D)
int f(int t, int B, int C, int D) {
    if (t < 20)
        return (B & C) | ((~B) & D);
    else if (t < 40)
        return B ^ C ^ D;
    else if (t < 60)
        return (B & C) | (B & D) | (C & D);
    else
        return B ^ C ^ D;
}
//K(t)
int K(int t) {
    if (t < 20)
        return 0x5a827999;
    else if (t < 40)
        return 0x6ed9eba1;
    else if (t < 60)
        return 0x8f1bbcdc;
    else
        return 0xca62c1d6;
}
//循环左移
int S(unsigned int x, int n) {
    return x >> (32 - n) | (x << n);
}
//生成第n块明文的W数组
void setW(int W[], vector<unsigned char> m, int n) {
    n *= 64;
    for (int i = 0; i < 16; i++) {
        W[i] = (m[n + 4 * i] << 24) + (m[n + 4 * i + 1] << 16)
            + (m[n + 4 * i + 2] << 8) + m[n + 4 * i + 3];
    }
    for (int i = 16; i < 80; i++) {
        W[i] = S(W[i - 16] ^ W[i - 14] ^ W[i - 8] ^ W[i - 3], 1);
    }
}

string sha1_hex(string text) {
    int A, B, C, D, E;
    int H0, H1, H2, H3, H4;
    vector<unsigned char> message;
    int n;
    int W[80];
    H0 = A = 0x67452301;
    H1 = B = 0xefcdab89;
    H2 = C = 0x98badcfe;
    H3 = D = 0x10325476;
    H4 = E = 0xc3d2e1f0;
    padding(message, n, text);
    for (int i = 0; i < n; i++) {
        setW(W, message, i);
        for (int t = 0; t < 80; t++) {
            int temp = E + f(t, B, C, D) + S(A, 5) + W[t] + K(t);
            E = D;
            D = C;
            C = S(B, 30);
            B = A;
            A = temp;
        }
        //将伪代码中A = H0与H0 = H0 + A合并为H0 = A = A + H0一行代码，其他变量同理。
        H0 = A = A + H0;
        H1 = B = B + H1;
        H2 = C = C + H2;
        H3 = D = D + H3;
        H4 = E = E + H4;
    }
    stringstream ss;
    ss << hex << setfill('0') << setw(8) << H0 << H1 << H2 << H3 << H4;
    return ss.str();
}

