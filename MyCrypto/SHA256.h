#pragma once
#include"HashSuperClass.h"
class SHA256 :public HashSuperClass
{
public:
	SHA256();
	SHA256(string s);
	string hexdigest();
};

