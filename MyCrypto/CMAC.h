#pragma once
#include<string>
using std::string;
class CMAC
{
private:
	string key;
	string msg;
public:
	CMAC(string key_, string msg);
	CMAC(string key_);
	~CMAC();
	CMAC& operator=(CMAC a);
	void update(string s);
	string hexdigest();
	bool hexverify(string hexdigest);
};

