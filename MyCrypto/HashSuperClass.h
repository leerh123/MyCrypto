#pragma once
#include<string>
using namespace std;
//����Hash�����ĸ���
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

