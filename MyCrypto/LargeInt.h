#pragma once
#include"number_support.h"
#include<iostream>
#include<array>
#include<string>
using std::ostream;
using std::array;
using std::string;

class LargeInt
{
	friend void swap(LargeInt&, LargeInt&);

	friend LargeInt operator+(const LargeInt&, const LargeInt&);
	friend LargeInt operator+(const LargeInt&, const lint&);
	friend LargeInt operator-(const LargeInt&, const LargeInt&);//为了效率，尽量不要传入差为负的两个数
	friend LargeInt operator-(const LargeInt&, const lint&);

	friend bool operator>(const LargeInt&, const LargeInt&);
	friend bool operator<(const LargeInt&, const LargeInt&);
	friend bool operator==(const LargeInt&, const LargeInt&);
	friend bool operator!=(const LargeInt&, const LargeInt&);
	friend bool operator>=(const LargeInt&, const LargeInt&);
	friend bool operator<=(const LargeInt&, const LargeInt&);

	friend LargeInt mul(const LargeInt& _a, const LargeInt& _b);
	friend LargeInt sqr(const LargeInt& _a);
	friend LargeInt karastuba_mul(const LargeInt& _a, const LargeInt& _b);
	friend LargeInt karastuba_mul(lint* lsd_a, lint* lsd_b, lint len_a, lint len_b);
	friend LargeInt operator*(const LargeInt&, const LargeInt&);
	friend LargeInt operator*(const LargeInt&, const lint&);

	friend LargeInt operator%(const LargeInt&, const LargeInt&);
	friend LargeInt operator%(const LargeInt&, const lint&);
	friend LargeInt operator/(const LargeInt&, const LargeInt&);
	friend LargeInt operator/(const LargeInt&, const lint&);

	friend LargeInt operator&(const LargeInt& _a, const LargeInt& _b);
	friend LargeInt operator^(const LargeInt& _a, const LargeInt& _b);
	friend LargeInt operator|(const LargeInt& _a, const LargeInt& _b);
	friend LargeInt operator<<(const LargeInt& _a, unsigned int _b);
	friend LargeInt operator>>(const LargeInt& _a, unsigned int _b);

	friend ostream& operator<<(ostream&, const LargeInt&);


public:
	LargeInt();
	LargeInt(unsigned long);
	LargeInt(unsigned int);
	LargeInt(unsigned short);
	LargeInt(long);
	LargeInt(int);
	LargeInt(short);
	LargeInt(const LargeInt&);
	LargeInt(LargeInt&&) noexcept;
	LargeInt(const lint*);
	//输入一个十六进制字符串
	LargeInt(const std::string&);
	LargeInt(const char*);
	LargeInt& operator=(LargeInt);
	~LargeInt();

	void set_max();

	LargeInt& operator++();
	LargeInt operator++(int);
	LargeInt& operator+=(const LargeInt&);
	LargeInt& operator+=(lint);
	LargeInt& operator--();
	LargeInt operator--(int);
	LargeInt& operator-=(const LargeInt&);
	LargeInt& operator-=(lint);

	LargeInt& operator *=(const LargeInt&);
	LargeInt& operator *=(lint);

	LargeInt& operator %=(const LargeInt&);
	LargeInt& operator %=(lint);
	LargeInt& operator /=(const LargeInt&);
	LargeInt& operator /=(lint);

	LargeInt& operator &=(const LargeInt& _a);
	LargeInt& operator ^=(const LargeInt& _a);
	LargeInt& operator |=(const LargeInt& _a);
	LargeInt& operator <<=(unsigned int _a);
	LargeInt& operator >>=(unsigned int _a);

	bool iseven();
	bool isodd();
	string hex_str();

	lint* copyvalue();
	lint* copyvalue() const;
	lint* value();
	const lint* value() const;
	unsigned int id_l();
private:
	lint* number_ = nullptr;

	//左移一位
	void left_shift();
	//右移一位
	void right_shift();

};
void swap(LargeInt&, LargeInt&);

LargeInt operator+(const LargeInt&, const LargeInt&);
LargeInt operator+(const LargeInt&, const lint&);
LargeInt operator-(const LargeInt&, const LargeInt&);
LargeInt operator-(const LargeInt&, const lint&);

bool operator>(const LargeInt&, const LargeInt&);
bool operator<(const LargeInt&, const LargeInt&);
bool operator==(const LargeInt&, const LargeInt&);
bool operator!=(const LargeInt&, const LargeInt&);
bool operator>=(const LargeInt&, const LargeInt&);
bool operator<=(const LargeInt&, const LargeInt&);


LargeInt mul(const LargeInt& _a, const LargeInt& _b);
LargeInt sqr(const LargeInt& _a);
LargeInt operator*(const LargeInt&, const LargeInt&);
LargeInt operator*(const LargeInt&, const lint&);


LargeInt operator%(const LargeInt&, const LargeInt&);
LargeInt operator%(const LargeInt&, const lint&);
LargeInt operator/(const LargeInt&, const LargeInt&);
LargeInt operator/(const LargeInt&, const lint&);

LargeInt operator&(const LargeInt& _a, const LargeInt& _b);
LargeInt operator^(const LargeInt& _a, const LargeInt& _b);
LargeInt operator|(const LargeInt& _a, const LargeInt& _b);
LargeInt operator<<(const LargeInt& _a, unsigned int _b);
LargeInt operator>>(const LargeInt& _a, unsigned int _b);


ostream& operator<<(ostream&, const LargeInt&);



