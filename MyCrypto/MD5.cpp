#include "MD5.h"
string md5_hex(string );
MD5::MD5():HashSuperClass(){}
MD5::MD5(string s):HashSuperClass(s){}
string MD5::hexdigest() {
	return md5_hex(text);
}

#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>
using namespace std;

#define F(x,y,z) ((x & y) | (~x & z))  
#define G(x,y,z) ((x & z) | (y & ~z))  
#define H(x,y,z) (x^y^z)  
#define I(x,y,z) (y ^ (x | ~z))  
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))  

#define FF(a,b,c,d,x,s,ac) { \
a += F(b, c, d) + x + ac; \
a = ROTATE_LEFT(a, s); \
a += b; \
}

#define GG(a,b,c,d,x,s,ac) { \
	a += G(b, c, d) + x + ac; \
	a = ROTATE_LEFT(a, s); \
	a += b; \
}

#define HH(a,b,c,d,x,s,ac) { \
	a += H(b, c, d) + x + ac; \
	a = ROTATE_LEFT(a, s); \
	a += b; \
}
#define II(a,b,c,d,x,s,ac) { \
	a += I(b, c, d) + x + ac; \
	a = ROTATE_LEFT(a, s); \
	a += b; \
}


//��亯��
//mΪԭʼ���ģ���������ݳ���Ϊ512*n bit�������������output�б���
static void padding(vector<unsigned char>& output, int& n, string m) {
	n = (m.size() + 8) / 64 + 1;
	output.resize(n * 64);
	int i = 0;
	//ԭʼ������Ϣ
	for (; i < m.size(); i++) {
		output[i] = m[i];
	}
	//���10...0
	output[i++] = 0x80;
	while (i < output.size() - 8) {
		output[i] = 0;
		i++;
	}
	//��䳤��
	long long int len = m.size() * 8;
	for (i = output.size() - 8; i < output.size(); ++i) {
		output[i] = len % 256;
		len /= 256;
	}

}

//���ɵ�n���X����
void setX(unsigned int X[], vector<unsigned char> m, int n) {
	n *= 64;
	for (int i = 0; i < 16; i++) {
		X[i] = (m[n + 4 * i + 3] << 24) + (m[n + 4 * i + 2] << 16)
			+ (m[n + 4 * i + 1] << 8) + m[n + 4 * i];
	}
}

string md5_hex(string text) {
	vector<unsigned char> message;//�������Ϣ
	int n;//��512����һ�飬message���Է�Ϊn��
	padding(message, n, text);

	unsigned int A, B, C, D;
	unsigned int AA, BB, CC, DD;
	unsigned int X[16];
	A = 0x67452301;
	B = 0xEFCDAB89;
	C = 0x98BADCFE;
	D = 0x10325476;
	for (int i = 0; i < n; ++i) {
		setX(X, message, i);
		AA = A, BB = B, CC = C, DD = D;

		FF(A, B, C, D, X[0], 7, 0xd76aa478);
		FF(D, A, B, C, X[1], 12, 0xe8c7b756);
		FF(C, D, A, B, X[2], 17, 0x242070db);
		FF(B, C, D, A, X[3], 22, 0xc1bdceee);
		FF(A, B, C, D, X[4], 7, 0xf57c0faf);
		FF(D, A, B, C, X[5], 12, 0x4787c62a);
		FF(C, D, A, B, X[6], 17, 0xa8304613);
		FF(B, C, D, A, X[7], 22, 0xfd469501);
		FF(A, B, C, D, X[8], 7, 0x698098d8);
		FF(D, A, B, C, X[9], 12, 0x8b44f7af);
		FF(C, D, A, B, X[10], 17, 0xffff5bb1);
		FF(B, C, D, A, X[11], 22, 0x895cd7be);
		FF(A, B, C, D, X[12], 7, 0x6b901122);
		FF(D, A, B, C, X[13], 12, 0xfd987193);
		FF(C, D, A, B, X[14], 17, 0xa679438e);
		FF(B, C, D, A, X[15], 22, 0x49b40821);


		GG(A, B, C, D, X[1], 5, 0xf61e2562);
		GG(D, A, B, C, X[6], 9, 0xc040b340);
		GG(C, D, A, B, X[11], 14, 0x265e5a51);
		GG(B, C, D, A, X[0], 20, 0xe9b6c7aa);
		GG(A, B, C, D, X[5], 5, 0xd62f105d);
		GG(D, A, B, C, X[10], 9, 0x2441453);
		GG(C, D, A, B, X[15], 14, 0xd8a1e681);
		GG(B, C, D, A, X[4], 20, 0xe7d3fbc8);
		GG(A, B, C, D, X[9], 5, 0x21e1cde6);
		GG(D, A, B, C, X[14], 9, 0xc33707d6);
		GG(C, D, A, B, X[3], 14, 0xf4d50d87);
		GG(B, C, D, A, X[8], 20, 0x455a14ed);
		GG(A, B, C, D, X[13], 5, 0xa9e3e905);
		GG(D, A, B, C, X[2], 9, 0xfcefa3f8);
		GG(C, D, A, B, X[7], 14, 0x676f02d9);
		GG(B, C, D, A, X[12], 20, 0x8d2a4c8a);


		HH(A, B, C, D, X[5], 4, 0xfffa3942);
		HH(D, A, B, C, X[8], 11, 0x8771f681);
		HH(C, D, A, B, X[11], 16, 0x6d9d6122);
		HH(B, C, D, A, X[14], 23, 0xfde5380c);
		HH(A, B, C, D, X[1], 4, 0xa4beea44);
		HH(D, A, B, C, X[4], 11, 0x4bdecfa9);
		HH(C, D, A, B, X[7], 16, 0xf6bb4b60);
		HH(B, C, D, A, X[10], 23, 0xbebfbc70);
		HH(A, B, C, D, X[13], 4, 0x289b7ec6);
		HH(D, A, B, C, X[0], 11, 0xeaa127fa);
		HH(C, D, A, B, X[3], 16, 0xd4ef3085);
		HH(B, C, D, A, X[6], 23, 0x4881d05);
		HH(A, B, C, D, X[9], 4, 0xd9d4d039);
		HH(D, A, B, C, X[12], 11, 0xe6db99e5);
		HH(C, D, A, B, X[15], 16, 0x1fa27cf8);
		HH(B, C, D, A, X[2], 23, 0xc4ac5665);


		II(A, B, C, D, X[0], 6, 0xf4292244);
		II(D, A, B, C, X[7], 10, 0x432aff97);
		II(C, D, A, B, X[14], 15, 0xab9423a7);
		II(B, C, D, A, X[5], 21, 0xfc93a039);
		II(A, B, C, D, X[12], 6, 0x655b59c3);
		II(D, A, B, C, X[3], 10, 0x8f0ccc92);
		II(C, D, A, B, X[10], 15, 0xffeff47d);
		II(B, C, D, A, X[1], 21, 0x85845dd1);
		II(A, B, C, D, X[8], 6, 0x6fa87e4f);
		II(D, A, B, C, X[15], 10, 0xfe2ce6e0);
		II(C, D, A, B, X[6], 15, 0xa3014314);
		II(B, C, D, A, X[13], 21, 0x4e0811a1);
		II(A, B, C, D, X[4], 6, 0xf7537e82);
		II(D, A, B, C, X[11], 10, 0xbd3af235);
		II(C, D, A, B, X[2], 15, 0x2ad7d2bb);
		II(B, C, D, A, X[9], 21, 0xeb86d391);

		A = A + AA, B = B + BB, C = C + CC, D = D + DD;
	}
	stringstream ss;
	ss << hex << setfill('0') << setw(2)
		<< (A & 0xffU) << ((A >> 8) & 0xffU) << ((A >> 16) & 0xffU) << ((A >> 24) & 0xffU)
		<< (B & 0xffU) << ((B >> 8) & 0xffU) << ((B >> 16) & 0xffU) << ((B >> 24) & 0xffU)
		<< (C & 0xffU) << ((C >> 8) & 0xffU) << ((C >> 16) & 0xffU) << ((C >> 24) & 0xffU)
		<< (D & 0xffU) << ((D >> 8) & 0xffU) << ((D >> 16) & 0xffU) << ((D >> 24) & 0xffU);
	return ss.str();
}