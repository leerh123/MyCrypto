#include "HMAC.h"
#include"hash.h"
#include<string>
#include<cstring>
using namespace std;
HMAC::HMAC(string key_, string msg_, string digestmod_):msg(msg_),digestmod(digestmod_) {
	key = key_;
	key.resize(64);
}
HMAC::HMAC(string key_, string digestmod_) :digestmod(digestmod_) {
	key = key_;
	key.resize(64);
}

HMAC::~HMAC() {}

HMAC& HMAC::operator=(HMAC a) {
	this->key = a.key;
	this->msg = a.msg;
	this->digestmod = a.digestmod;
	return *this;
}

void HMAC::update(string s) {
	msg += s;
}

string HMAC::hexdigest() {
	//H(k^opad,H(k^ipad,msg))
	//k_xor_ipad = k^ipad ,  k_xor_opad = k^opad 
	//ipad_msg = (k^ipad,m)  , ipad_hash = H(ipad_msg)
	//opad_msg = (k^opad,H(k^ipad,msg)) , res = H(opad_msg)
	string k_xor_opad, k_xor_ipad;
	k_xor_opad.resize(64);
	k_xor_ipad.resize(64);
	for (int i = 0; i < 64; ++i) {
		k_xor_opad[i] = key[i] ^ 0x5c;
		k_xor_ipad[i] = key[i] ^ 0x36;
	}
	string ipad_msg = k_xor_ipad + msg;
	string ipad_hash, opad_msg, res;
	if (digestmod == "MD5") {
		auto H = MD5(ipad_msg);
		string ipad_hash = H.hexdigest();

		opad_msg = k_xor_opad + ipad_hash;
		auto H1 = MD5(opad_msg);
		res = H1.hexdigest();

	}
	else if (digestmod == "SHA1") {
		auto H = SHA1(ipad_msg);
		string ipad_hash = H.hexdigest();

		opad_msg = k_xor_opad + ipad_hash;
		auto H1 = SHA1(opad_msg);
		res = H1.hexdigest();
	}
	else if (digestmod == "SHA3_256") {
		auto H = SHA3_256(ipad_msg);
		string ipad_hash = H.hexdigest();

		opad_msg = k_xor_opad + ipad_hash;
		auto H1 = SHA3_256(opad_msg);
		res = H1.hexdigest();
	}
	else if (digestmod == "SHA256") {
		auto H = SHA256(ipad_msg);
		string ipad_hash = H.hexdigest();

		opad_msg = k_xor_opad + ipad_hash;
		auto H1 = SHA256(opad_msg);
		res = H1.hexdigest();
	}
	else if (digestmod == "SHA512") {
		auto H = SHA512(ipad_msg);
		string ipad_hash = H.hexdigest();

		opad_msg = k_xor_opad + ipad_hash;
		auto H1 = SHA512(opad_msg);
		res = H1.hexdigest();
	}
	else if (digestmod == "SM3") {
		auto H = SM3(ipad_msg);
		string ipad_hash = H.hexdigest();

		opad_msg = k_xor_opad + ipad_hash;
		auto H1 = SM3(opad_msg);
		res = H1.hexdigest();
	}
	else {
		auto H = MD5(ipad_msg);
		string ipad_hash = H.hexdigest();

		opad_msg = k_xor_opad + ipad_hash;
		auto H1 = MD5(opad_msg);
		res = H1.hexdigest();
	}
	return res;

}
bool HMAC::hexverify(string mac) {
	return (hexdigest() == mac);
}