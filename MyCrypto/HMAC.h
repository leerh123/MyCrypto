#pragma once
#include<string>
using std::string;
class HMAC
{
private:
	string key;
	string digestmod;
	string msg;
public:
	HMAC(string key_, string msg, string digestmod_ = "MD5");
	HMAC(string key_, string digestmod_ = "MD5");
	~HMAC();
	HMAC& operator=(HMAC a);
	void update(string s);
	string hexdigest();
	bool hexverify(string hexdigest);
};

