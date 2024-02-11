#pragma once
#include<string>
using namespace std;
//所有Hash函数的父类
class HashSuperClass
{
protected:
	string text;
public:
	HashSuperClass();
	HashSuperClass(string s);
	void update(string s);
	virtual string hexdigest() = 0;
};

