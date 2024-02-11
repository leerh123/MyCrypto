#pragma once
#include"LargeInt.h"
#include<utility>
using std::pair;
class RSA
{
private:
	LargeInt e, d;
	LargeInt p, q;
	LargeInt n, phi;
public:
	RSA(size_t N = 1024);
	void generate(size_t N);//Éú³É¹«Ë½Ô¿
	LargeInt encrypt(string message);
	string decrypt(LargeInt c);
	void import_p_q(LargeInt p_, LargeInt q_);
	pair<LargeInt, LargeInt> export_public_key();
	pair<LargeInt, LargeInt> export_private_key();


};
LargeInt str2largeint(string str);
string largeint2str(LargeInt num);
