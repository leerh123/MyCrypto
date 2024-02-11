#pragma once
#include"HashSuperClass.h"
class SHA512 :public HashSuperClass
{
public:
	SHA512();
	SHA512(string s);
	string hexdigest();
};

