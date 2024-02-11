#pragma once
#include"HashSuperClass.h"
class SM3 :public HashSuperClass
{
public:
	SM3();
	SM3(string s);
	string hexdigest();
};
