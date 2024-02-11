#pragma once
#include"HashSuperClass.h"
class MD5:public HashSuperClass
{
public:
	MD5();
	MD5(string s);
	string hexdigest();
};

