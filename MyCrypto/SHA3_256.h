#pragma once
#include"HashSuperClass.h"
class SHA3_256 :public HashSuperClass
{
public:
	SHA3_256();
	SHA3_256(string s);
	string hexdigest();
};
