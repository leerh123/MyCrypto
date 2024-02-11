#pragma once
#include"HashSuperClass.h"
class SHA1:public HashSuperClass
{
public:
	SHA1();
	SHA1(string s);
	string hexdigest();
};

