#include "RSA.h"
#include"Random.h"
#include"Mod.h"
#include<sstream>
#include<cmath>
using namespace std;
RSA::RSA(size_t N) {
	generate(N);
}
void RSA::generate(size_t N) {
	size_t size_p = N / 2;
	size_t size_q = N - size_p;
	p = get_random_prime(size_p);
	q = get_random_prime(size_q);
	n = p * q;
	phi = (p - 1) * (q - 1);
	e = 65537;
	d = inverse(e, phi);
}
//LargeInt RSA::encrypt(LargeInt m) {
//	return LargeInt_pow(m, e, n);
//}
//
//
//LargeInt RSA::decrypt(LargeInt c) {
//	return  LargeInt_pow(c, d, n);
//}
LargeInt RSA::encrypt(string str) {
	LargeInt m = str2largeint(str);
	return LargeInt_pow(m, e, n);
}


string RSA::decrypt(LargeInt c) {
	LargeInt m = LargeInt_pow(c, d, n);
	return  largeint2str(m);
}

void RSA::import_p_q(LargeInt p_, LargeInt q_) {
	p = p_;
	q = q_;
	n = p * q;
	phi = (p - 1) * (q - 1);
	e = 65537;
	d = inverse(e, phi);
}
pair<LargeInt, LargeInt> RSA::export_public_key() {
	return pair<LargeInt, LargeInt>(n, e);
}
pair<LargeInt, LargeInt> RSA::export_private_key() {
	return pair<LargeInt, LargeInt>(n, d);
}

LargeInt str2largeint(string str) {
	stringstream ss;
	for (size_t i = 0; i < str.size(); i++) {
		ss << hex << int(str[i]);
	}
	return LargeInt(ss.str());
}
string largeint2str(LargeInt num) {
	string num_hex = num.hex_str();
	if (num_hex.size() % 2 != 0)
		num_hex.insert(0, 1, '0');
	string sub_num_hex;
	char c;
	stringstream res;
	for (size_t i = 0; i < num_hex.size() / 2; ++i) {
		sub_num_hex = num_hex.substr(2 * i, 2);
		c = char(stoi(sub_num_hex,nullptr,16));
		res << c;
	}

	return res.str();
}