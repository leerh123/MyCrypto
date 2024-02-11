#include "LargeInt.h"
#include"number_support.h"
#include<algorithm>
#include<string>
#include<sstream>
#include<iomanip>
using namespace std;
LargeInt::LargeInt() {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	number_[0] = kLINTMAXDIGIT;
}

LargeInt::LargeInt(unsigned long _ul) {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	u2lint_l(number_, _ul);
}

LargeInt::LargeInt(unsigned int _i) {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	u2lint_l(number_, (unsigned long)_i);
}

LargeInt::LargeInt(unsigned short _u) {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	u2lint_l(number_, _u);
}

LargeInt::LargeInt(long _i) {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	u2lint_l(number_, (unsigned long)_i);
}

LargeInt::LargeInt(int _i) {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	u2lint_l(number_, (unsigned long)_i);
}

LargeInt::LargeInt(short _i) {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	u2lint_l(number_, (unsigned long)_i);
}

LargeInt::LargeInt(const LargeInt& _b) {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	for (size_t i = 0; i <= DIGITS_L(_b.number_); ++i) {
		number_[i] = _b.number_[i];
	}
}

LargeInt::LargeInt(LargeInt&& _b) noexcept {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	for (size_t i = 0; i <= DIGITS_L(_b.number_); ++i) {
		number_[i] = _b.number_[i];
	}
}

LargeInt::LargeInt(const lint* _b) {
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	memcpy(number_, _b, min((const unsigned int)(DIGITS_L(_b) + 1), kLINTMAXSHORT) * sizeof(lint));
	/*for (size_t i = 0; i <= DIGITS_L(_b); ++i) {
		number_[i] = _b[i];
	}*/
}


LargeInt::LargeInt(const std::string& _s) {
	try {
		if (_s.size() > (kLINTMAXBIT / 4))
			throw invalid_argument("the string is too long");
	}
	catch (invalid_argument e) {
		cerr << e.what() << endl;
		exit(1);
	}
	if (number_ != nullptr) {
		delete[] number_;
	}
	number_ = new LINT;
	str2LINT_l(number_, _s);
}

LargeInt::LargeInt(const char* _s) {
	const string& str = _s;
	*this = LargeInt(str);
}

LargeInt& LargeInt::operator=(LargeInt _b) {
	swap(*this, _b);
	return *this;
}

LargeInt::~LargeInt() {
	delete[] number_;
}



void swap(LargeInt& _a, LargeInt& _b) {
	using std::swap;
	swap(_a.number_, _b.number_);
}

void LargeInt::set_max() {
	SETMAX_L(number_);
}


LargeInt operator+(const LargeInt& _a, const LargeInt& _b) {
	lint result[130];
	const lint* longer_number_msd_ptr, * longer_number_lsd_ptr;
	const lint* shorter_number_msd_ptr, * shorter_number_lsd_ptr;
	lint* result_msd_ptr = LSDPTR_L(result);
	ULONG carry = 0x0;
	if (DIGITS_L(_a.number_) < DIGITS_L(_b.number_)) {
		shorter_number_lsd_ptr = LSDPTR_L(_a.number_);
		shorter_number_msd_ptr = MSDPTR_L(_a.number_);
		longer_number_lsd_ptr = LSDPTR_L(_b.number_);
		longer_number_msd_ptr = MSDPTR_L(_b.number_);
		SETDIGITS_L(result, DIGITS_L(_b.number_));
	}
	else {
		shorter_number_lsd_ptr = LSDPTR_L(_b.number_);
		shorter_number_msd_ptr = MSDPTR_L(_b.number_);
		longer_number_lsd_ptr = LSDPTR_L(_a.number_);
		longer_number_msd_ptr = MSDPTR_L(_a.number_);
		SETDIGITS_L(result, DIGITS_L(_a.number_));
	}
	for (; shorter_number_lsd_ptr <= shorter_number_msd_ptr; ++shorter_number_lsd_ptr, ++longer_number_lsd_ptr, ++result_msd_ptr) {
		*result_msd_ptr = (USHORT)(carry = (ULONG)(*shorter_number_lsd_ptr)
			+ (ULONG)(*longer_number_lsd_ptr)
			+ (ULONG)(USHORT)(carry>>kBITPERDIG));
	}
	for (; longer_number_lsd_ptr <= longer_number_msd_ptr; ++longer_number_lsd_ptr, ++result_msd_ptr) {
		*result_msd_ptr = (USHORT)(carry = (ULONG)(*longer_number_lsd_ptr)
			+ (ULONG)(USHORT)(carry >> kBITPERDIG));
	}
	if (carry & kBASE) {
		*result_msd_ptr = 1;
		INCDIGITS_L(result);


	}
	if (DIGITS_L(result) > (lint)(kLINTMAXDIGIT)) {
		ANDMAX_L(result);
	}
	//可以优化吗？
	//LINT result_final;
	//memcpy(result_final, result, (result[0]+1) * sizeof(lint));
	//return LargeInt(result_final);
	return LargeInt(result);
}

LargeInt operator+(const LargeInt& _a, const lint& _b) {
	return _a + LargeInt(_b);
}

LargeInt& LargeInt::operator++() {
	*this = *this + 1;
	return *this;
}

LargeInt LargeInt::operator++(int) {
	LargeInt tmp(*this);
	*this = *this + 1;
	return tmp;
}

LargeInt& LargeInt::operator+=(const LargeInt& _a) {
	*this = *this + _a;
	return *this;
}

LargeInt& LargeInt::operator+=(lint _a) {
	*this = *this + _a;
	return *this;
}

LargeInt operator-(const LargeInt& _a, const LargeInt& _b) {
	LINT result;
	lint* a = _a.copyvalue();
	lint* b = _b.copyvalue();
	const lint* a_msd_ptr, * b_msd_ptr;
	lint* result_lsd_ptr;
	const lint* a_lsd_ptr, * b_lsd_ptr;
	a_msd_ptr = MSDPTR_L(a);
	b_msd_ptr = MSDPTR_L(b);
	result_lsd_ptr = LSDPTR_L(result);
	a_lsd_ptr = LSDPTR_L(a);
	b_lsd_ptr = LSDPTR_L(b);
	ULONG  carry = 0x0;
	bool underflow = false;
	if (cmp_l(a, b) == -1) {
		SETMAX_L(a);
		a_msd_ptr = a + kLINTMAXDIGIT;
		SETDIGITS_L(result, kLINTMAXDIGIT);
		underflow = true;
	}
	else {
		SETDIGITS_L(result, DIGITS_L(a));
	}
	for (; b_lsd_ptr <= b_msd_ptr; ++result_lsd_ptr, ++a_lsd_ptr, ++b_lsd_ptr) {
		*result_lsd_ptr = (USHORT)(carry = (ULONG)(*a_lsd_ptr)
			- (ULONG)(*b_lsd_ptr)
			- ((carry & kBASE) >> kBITPERDIG));
	}
	for (; a_lsd_ptr <= a_msd_ptr; ++result_lsd_ptr, ++a_lsd_ptr) {
		*result_lsd_ptr = (USHORT)(carry = (ULONG)(*a_lsd_ptr)
			- ((carry & kBASE) >> kBITPERDIG));
	}
	RMLDZRS_L(result);
	if (underflow) {
		LargeInt res = LargeInt(result) + _a + 1;
		delete[]a;
		delete[]b;
		return res;

	}

	delete[]a;
	delete[]b;

	return LargeInt(result);
}

LargeInt operator-(const LargeInt& _a, const lint& _b) {
	return _a - LargeInt(_b);
}

LargeInt& LargeInt::operator--() {
	*this = *this - 1;
	return *this;
}

LargeInt LargeInt::operator--(int) {
	LargeInt tmp(*this);
	*this = *this - 1;
	return tmp;
}

LargeInt& LargeInt::operator-=(const LargeInt& _a) {
	*this = *this - _a;
	return *this;
}

LargeInt& LargeInt::operator-=(lint _a) {
	*this = *this - _a;
	return *this;
}



bool operator>(const LargeInt& _a, const LargeInt& _b) {
	return GT_L(_a.number_, _b.number_);
}

bool operator<(const LargeInt& _a, const LargeInt& _b) {
	return LT_L(_a.number_, _b.number_);
}

bool operator==(const LargeInt& _a, const LargeInt& _b) {
	return equ_l(_a.number_, _b.number_);
}

bool operator!=(const LargeInt& _a, const LargeInt& _b) {
	return !(_a == _b);
}

bool operator>=(const LargeInt& _a, const LargeInt& _b) {
	return GE_L(_a.number_, _b.number_);
}

bool operator<=(const LargeInt& _a, const LargeInt& _b) {
	return !(_a > _b);
}



LargeInt mul(const LargeInt& _a, const LargeInt& _b) {
	LINTD result = { 0 };
	const lint* a = _a.number_;
	const lint* b = _b.number_;
	const lint* longer_number, * shorter_number;
	lint* result_lsd_ptr = result + 1;
	ULONG carry = 0x0;
	if (EQZ_L(a) || EQZ_L(b)) {
		return LargeInt(0);
	}
	if (DIGITS_L(a) < DIGITS_L(b)) {
		longer_number = b;
		shorter_number = a;
	}
	else {
		longer_number = a;
		shorter_number = b;
	}
	const lint* longer_number_msd_ptr = longer_number + *longer_number;
	const lint* shorter_number_msd_ptr = shorter_number + *shorter_number;
	const lint* longer_number_lsd_ptr = longer_number + 1;
	const lint* shorter_number_lsd_ptr = shorter_number + 1;
	lint av = *longer_number_lsd_ptr;
	for (; shorter_number_lsd_ptr <= shorter_number_msd_ptr; ++shorter_number_lsd_ptr, ++result_lsd_ptr) {
		*result_lsd_ptr = (USHORT)(carry = (ULONG)(av) * (ULONG)(*shorter_number_lsd_ptr)
			+ (ULONG)(USHORT)(carry >> kBITPERDIG));
	}
	*result_lsd_ptr = (USHORT)(carry >> kBITPERDIG);

	lint* csptr = result + 2;
	const lint* aptr = longer_number + 2;
	for (; aptr <= longer_number_msd_ptr; ++csptr, ++aptr) {
		carry = 0x0;
		av = *aptr;
		for (shorter_number_lsd_ptr = shorter_number + 1, result_lsd_ptr = csptr; shorter_number_lsd_ptr <= shorter_number_msd_ptr; ++shorter_number_lsd_ptr, ++result_lsd_ptr) {
			*result_lsd_ptr = (USHORT)(carry = (ULONG)(av) * (ULONG)(*shorter_number_lsd_ptr)
				+ (ULONG)(*result_lsd_ptr) + (ULONG)(USHORT)(carry >> kBITPERDIG));
		}
		*result_lsd_ptr = (USHORT)(carry >> kBITPERDIG);
	}
	SETDIGITS_L(result, DIGITS_L(a) + DIGITS_L(b));
	while ((result[0] > 0) && (result[DIGITS_L(result)] == 0))
		--result[0];
	//上溢
	if (result[0] > (lint)(kLINTMAXDIGIT)) {
		result[0] = (lint)kLINTMAXDIGIT;
		while ((result[0] > 0) && (result[DIGITS_L(result)] == 0))
			--result[0];
	}

//	LINT result_final;
//	for (size_t i = 0; i <= result[0]; ++i) {
//		result_final[i] = result[i];
//}
//	return LargeInt(result_final);
	return LargeInt(result);
}

LargeInt sqr(const LargeInt& _a) {
	lint* a_l = _a.number_;
	LINTD p_l;
	lint* aptr_l, * csptr_l, * msdptra_l, * msdptrb_l, * msdptrc_l;
	lint* pptr_l, * bptr_l;
	USHORT av;
	ULONG carry;
	LINT pp_l;
	if (EQZ_L(a_l))
		return 0;
	msdptrb_l = MSDPTR_L(a_l);
	msdptra_l = msdptrb_l - 1;
	*LSDPTR_L(p_l) = 0;
	carry = 0;
	av = *LSDPTR_L(a_l);
	for (bptr_l = LSDPTR_L(a_l) + 1, pptr_l = LSDPTR_L(p_l) + 1; bptr_l <= msdptrb_l; ++bptr_l, ++pptr_l) {
		*pptr_l = (USHORT)(carry = (ULONG)av * (ULONG)(*bptr_l) + (ULONG)(USHORT)(carry >> kBITPERDIG));
	}
	*pptr_l = (USHORT)(carry >> kBITPERDIG);
	for (aptr_l = LSDPTR_L(a_l) + 1, csptr_l = LSDPTR_L(p_l) + 3; aptr_l <= msdptra_l; ++aptr_l, csptr_l += 2) {
		carry = 0;
		av = *aptr_l;
		for (bptr_l = aptr_l + 1, pptr_l = csptr_l; bptr_l <= msdptrb_l; ++bptr_l, ++pptr_l) {
			*pptr_l = (USHORT)(carry = (ULONG)av * (ULONG)(*bptr_l) + (ULONG)(*pptr_l) + (ULONG)(USHORT)(carry >> kBITPERDIG));
		}
		*pptr_l = (USHORT)(carry >> kBITPERDIG);
	}
	msdptrc_l = pptr_l;
	carry = 0;
	for (pptr_l = LSDPTR_L(p_l); pptr_l <= msdptrc_l; ++pptr_l) {
		*pptr_l = (USHORT)(carry = (((ULONG)*pptr_l) << 1) + (ULONG)(USHORT)(carry >> kBITPERDIG));
	}
	*pptr_l = (USHORT)(carry >> kBITPERDIG);
	carry = 0;
	for (bptr_l = LSDPTR_L(a_l), pptr_l = LSDPTR_L(p_l); bptr_l <= msdptrb_l; ++bptr_l, ++pptr_l) {
		*pptr_l = (USHORT)(carry = (ULONG)(*bptr_l) * (ULONG)(*bptr_l) + (ULONG)(*pptr_l) + (ULONG)(USHORT)(carry >> kBITPERDIG));
		++pptr_l;
		*pptr_l = (USHORT)(carry = (ULONG)(*pptr_l) + (carry >> kBITPERDIG));
	}
	SETDIGITS_L(p_l, DIGITS_L(a_l) << 1);
	RMLDZRS_L(p_l);
	if (DIGITS_L(p_l) > (lint)kLINTMAXDIGIT) {
		ANDMAX_L(p_l);
	}
	//for (size_t i = 0; i <= p_l[0]; ++i) {
	//	pp_l[i] = p_l[i];
	//}

	return LargeInt(p_l);
}

//输入两个加数的最低有效位指针，返回其最低有效位指针,len为result的长度
lint* addkar(lint* lsd_a, lint* lsd_b, int len_a,int len_b) {
	lint* result = new lint[130];
	const lint* longer_number_msd_ptr, * longer_number_lsd_ptr;
	const lint* shorter_number_msd_ptr, * shorter_number_lsd_ptr;
	lint* result_msd_ptr = LSDPTR_L(result);
	ULONG carry = 0x0;
	lint* msd_a = lsd_a + len_a - 1;
	lint* msd_b = lsd_b + len_b - 1;
	if (len_a < len_b) {
		shorter_number_lsd_ptr = lsd_a;
		shorter_number_msd_ptr = msd_a;
		longer_number_lsd_ptr = lsd_b;
		longer_number_msd_ptr = msd_b;
		SETDIGITS_L(result, len_b);
	}
	else {
		shorter_number_lsd_ptr = lsd_b;
		shorter_number_msd_ptr = msd_b;
		longer_number_lsd_ptr = lsd_a;
		longer_number_msd_ptr = msd_b;
		SETDIGITS_L(result, len_a);
	}
	for (; shorter_number_lsd_ptr <= shorter_number_msd_ptr; ++shorter_number_lsd_ptr, ++longer_number_lsd_ptr, ++result_msd_ptr) {
		*result_msd_ptr = (USHORT)(carry = (ULONG)(*shorter_number_lsd_ptr)
			+ (ULONG)(*longer_number_lsd_ptr)
			+ (ULONG)(USHORT)(carry >> kBITPERDIG));
	}
	for (; longer_number_lsd_ptr <= longer_number_msd_ptr; ++longer_number_lsd_ptr, ++result_msd_ptr) {
		*result_msd_ptr = (USHORT)(carry = (ULONG)(*longer_number_lsd_ptr)
			+ (ULONG)(USHORT)(carry >> kBITPERDIG));
	}
	if (carry & kBASE) {
		*result_msd_ptr = 1;
		INCDIGITS_L(result);


	}
	if (DIGITS_L(result) > (lint)(kLINTMAXDIGIT)) {
		ANDMAX_L(result);
	}
	return result;
}

LargeInt karastuba_mul(lint* lsd_a,lint* lsd_b,lint len_a,lint len_b) {
	if ((len_a == len_b) && (len_a >= 8)) {
		lint* lsd_a1, * lsd_b1;
		len_a /= 2;
		LargeInt c0 = karastuba_mul(lsd_a, lsd_b, len_a, len_a);
		lint tmp = len_b - len_a;
		lsd_a1 = lsd_a + len_a;
		lsd_b1 = lsd_b + len_a;
		LargeInt c1 = karastuba_mul(lsd_a1, lsd_b1, tmp, tmp);
		lint* a0_a1_p = addkar(lsd_a, lsd_a1, len_a, tmp);
		lint* b0_b1_p = addkar(lsd_b, lsd_b1, len_a, tmp);
		LargeInt c2 = karastuba_mul(a0_a1_p+1, b0_b1_p+1, *a0_a1_p, *b0_b1_p) - c0 - c1;
		delete[]a0_a1_p;
		delete[]b0_b1_p;
		return c0 + (c1 << (2 * len_a * kBITPERDIG)) + (c2 << (len_a * kBITPERDIG));
	}
	else {
		LargeInt a, b;
		SETDIGITS_L(a.number_, len_a);
		SETDIGITS_L(b.number_, len_b);
		memcpy(LSDPTR_L(a.number_), lsd_a, len_a * sizeof(lint));
		memcpy(LSDPTR_L(b.number_), lsd_b, len_b * sizeof(lint));
		return mul(a, b);

	}

}

LargeInt karastuba_mul(const LargeInt& _a, const LargeInt& _b) {
	int len_a = DIGITS_L(_a.number_);
	int len_b = DIGITS_L(_b.number_);
	//a,b长度相等，且a,b都大于256位时使用
	if ((len_a==len_b)&&(len_a>=8)) {
		len_a /= 2;
		//取低len_a位
		LargeInt a0(_a);
		LargeInt b0(_b);
		a0 = _a, b0 = _b;
		//取高len_a位
		LargeInt a1, b1;
		a1 = a0 >> (len_a * kBITPERDIG), b1 = b0 >> (len_a * kBITPERDIG);

		SETDIGITS_L(a0.number_, len_a), SETDIGITS_L(b0.number_, len_a);
		SETDIGITS_L(a1.number_, len_b - len_a), SETDIGITS_L(b1.number_, len_b - len_a);
		//c0:=a0b0,c1:=a1b1,c2:=(a0+a1)(b0+b1)-c0-c1
		LargeInt c0 = karastuba_mul(a0, b0);
		LargeInt c1 = karastuba_mul(a1, b1);
		LargeInt c2 = karastuba_mul(a0 + a1, b0 + b1) - c0 - c1;
		/*cout << "c0:" << c0 << endl;
		cout << "c1:" << c1 << endl;
		cout << "c2:" << c2 << endl;
		cout << "-----" << endl;*/
		//ab=B^k(B^k*c1+c2)+c0
		//return c0 + (c1 << (2 * len_a * kBITPERDIG)) + (c2 << (len_a * kBITPERDIG));
		LargeInt tmp = (c1 << (len_a * kBITPERDIG));
		//cout << "tmp:    "<<tmp << endl;
		tmp += c2;
		//cout << "tmp:    " << tmp << endl;
		LargeInt result = (tmp << (len_a * kBITPERDIG)) + c0;
		//cout << "result:    "<<result << endl;
		return c0 + (c1 << (2 * len_a * kBITPERDIG)) + (c2 << (len_a * kBITPERDIG));

	}
	else {
		return mul(_a, _b);
	}
}

LargeInt operator*(const LargeInt& _a, const LargeInt& _b) {
	if (_a == _b) {
		return sqr(_a);
	}
	//cout << "karastuba:"<<(karastuba_mul(_a, _b)==mul(_a,_b)) << endl;
	lint* lsd_a = LSDPTR_L(_a.number_);
	lint* lsd_b = LSDPTR_L(_b.number_);
	lint len_a = DIGITS_L(_a.number_);
	lint len_b = DIGITS_L(_b.number_);
	//cout << karastuba_mul(lsd_a, lsd_b, len_a, len_b) << endl;
	//cout << mul(_a, _b) << endl;
	//return karastuba_mul(lsd_a, lsd_b, len_a, len_b) == mul(_a, _b);
	//return karastuba_mul(lsd_a, lsd_b, len_a, len_b);
	//return karastuba_mul(_a, _b) == mul(_a, _b);
   //return karastuba_mul(_a, _b );
	return mul(_a, _b);
}

LargeInt operator*(const LargeInt& _a, const lint& _b) {
	return _a * LargeInt(_b);
}

LargeInt& LargeInt::operator *=(const LargeInt& _a) {
	*this = *this * _a;
	return *this;
}

LargeInt& LargeInt::operator *=(lint _a) {
	*this = *this * _a;
	return *this;
}



LargeInt operator&(const LargeInt& _a, const LargeInt& _b) {
	if ((vcheck_l(_a.number_) & vcheck_l(_b.number_)) == false) {
		throw invalid_argument("the BasicOp object is illegal");
	}
	LINT result;
	lint* shorter_number_lsd_ptr, * longer_number_lsd_ptr;
	lint* last_ptr;
	if (DIGITS_L(_a.number_) < DIGITS_L(_b.number_)) {
		shorter_number_lsd_ptr = LSDPTR_L(_a.number_);
		longer_number_lsd_ptr = LSDPTR_L(_b.number_);
		last_ptr = MSDPTR_L(_a.number_);
	}
	else {
		shorter_number_lsd_ptr = LSDPTR_L(_b.number_);
		longer_number_lsd_ptr = LSDPTR_L(_a.number_);
		last_ptr = MSDPTR_L(_b.number_);
	}

	SETDIGITS_L(result, *(shorter_number_lsd_ptr - 1));
	lint* result_ptr = LSDPTR_L(result);
	while (shorter_number_lsd_ptr <= last_ptr) {
		*result_ptr++ = *shorter_number_lsd_ptr++ & *longer_number_lsd_ptr++;
	}
	RMLDZRS_L(result);
	return LargeInt(result);

}

LargeInt operator^(const LargeInt& _a, const LargeInt& _b) {
	if ((vcheck_l(_a.number_) & vcheck_l(_b.number_)) == false) {
		throw invalid_argument("the BasicOp object is illegal");
	}
	LINT result;
	lint* shorter_number_lsd_ptr, * shorter_number_msd_ptr, * longer_number_lsd_ptr, * longer_number_msd_ptr, * result_ptr;
	if (DIGITS_L(_a.number_) < DIGITS_L(_b.number_)) {
		shorter_number_lsd_ptr = LSDPTR_L(_a.number_);
		longer_number_lsd_ptr = LSDPTR_L(_b.number_);
		shorter_number_msd_ptr = MSDPTR_L(_a.number_);
		longer_number_msd_ptr = MSDPTR_L(_b.number_);
	}
	else {
		shorter_number_lsd_ptr = LSDPTR_L(_b.number_);
		longer_number_lsd_ptr = LSDPTR_L(_a.number_);
		shorter_number_msd_ptr = MSDPTR_L(_b.number_);
		longer_number_msd_ptr = MSDPTR_L(_a.number_);
	}
	SETDIGITS_L(result, *(longer_number_lsd_ptr - 1));
	result_ptr = LSDPTR_L(result);
	while (shorter_number_lsd_ptr <= shorter_number_msd_ptr) {
		*result_ptr++ = *shorter_number_lsd_ptr++ ^ *longer_number_lsd_ptr++;
	}
	while (longer_number_lsd_ptr <= longer_number_msd_ptr) {
		*result_ptr++ = *longer_number_lsd_ptr++;
	}
	RMLDZRS_L(result);
	return LargeInt(result);
}

LargeInt operator|(const LargeInt& _a, const LargeInt& _b) {
	if ((vcheck_l(_a.number_) & vcheck_l(_b.number_)) == false) {
		throw invalid_argument("the BasicOp object is illegal");
	}
	LINT result;
	lint* shorter_number_lsd_ptr, * shorter_number_msd_ptr, * longer_number_lsd_ptr, * longer_number_msd_ptr, * result_ptr;
	if (DIGITS_L(_a.number_) < DIGITS_L(_b.number_)) {
		shorter_number_lsd_ptr = LSDPTR_L(_a.number_);
		longer_number_lsd_ptr = LSDPTR_L(_b.number_);
		shorter_number_msd_ptr = MSDPTR_L(_a.number_);
		longer_number_msd_ptr = MSDPTR_L(_b.number_);
	}
	else {
		shorter_number_lsd_ptr = LSDPTR_L(_b.number_);
		longer_number_lsd_ptr = LSDPTR_L(_a.number_);
		shorter_number_msd_ptr = MSDPTR_L(_b.number_);
		longer_number_msd_ptr = MSDPTR_L(_a.number_);
	}
	SETDIGITS_L(result, *(longer_number_lsd_ptr - 1));
	result_ptr = LSDPTR_L(result);
	while (shorter_number_lsd_ptr <= shorter_number_msd_ptr) {
		*result_ptr++ = *shorter_number_lsd_ptr++ | *longer_number_lsd_ptr++;
	}
	while (longer_number_lsd_ptr <= longer_number_msd_ptr) {
		*result_ptr++ = *longer_number_lsd_ptr++;
	}
	RMLDZRS_L(result);
	return LargeInt(result);
}

LargeInt& LargeInt::operator &=(const LargeInt& _a) {
	*this = *this & _a;
	return *this;
}

LargeInt& LargeInt::operator ^=(const LargeInt& _a) {
	*this = *this ^ _a;
	return *this;
}

LargeInt& LargeInt::operator |=(const LargeInt& _a) {
	*this = *this | _a;
	return *this;
}

void LargeInt::left_shift() {
	RMLDZRS_L(number_);
	lint* ap_l, * msdptra_l;
	ULONG carry = 0;
	msdptra_l = MSDPTR_L(number_);
	for (ap_l = LSDPTR_L(number_); ap_l <= msdptra_l; ap_l++) {
		*ap_l = (USHORT)(carry = ((ULONG)*ap_l << 1) | (carry >> kBITPERDIG));
	}
	if (carry >> kBITPERDIG) {
		if (DIGITS_L(number_) < kLINTMAXDIGIT) {
			*ap_l = 1;
			INCDIGITS_L(number_);
		}
		else {
			//throw runtime_error("the left_shift function's result is overflow");
		}
	}
}

void LargeInt::right_shift() {
	lint* ap_l, * lsdptra_l;
	USHORT carry = 0;
	USHORT help;
	lsdptra_l = LSDPTR_L(number_);
	if (EQZ_L(number_)) {
		return;
	}
	for (ap_l = MSDPTR_L(number_); ap_l >= lsdptra_l; ap_l--) {
		help = (USHORT)((USHORT)(*ap_l >> 1) | (USHORT)(carry << (kBITPERDIG - 1)));
		carry = (USHORT)(*ap_l & 1);
		*ap_l = help;
	}
	RMLDZRS_L(number_);

}

LargeInt operator<<(const LargeInt& _a, unsigned int _b) {
	if (vcheck_l(_a.number_) == false) {
		throw invalid_argument("the BasicOp object is illegal");
	}
	LargeInt result = _a;
	if (_b > kLINTMAXBIT) {
		return 0;
	}
	
	auto size = min((USHORT)_b / kBITPERDIG,(USHORT)kLINTMAXDIGIT-result.number_[0]);
	
	for (size_t i = result.number_[0]; i >=1 ; --i) {
		result.number_[i + size] = result.number_[i];
	}

	for (size_t i = 1; i <= size; ++i) {
		result.number_[i] = 0;
	}
	result.number_[0] += size;
	
	for (size_t i = 0; i < _b-(size*kBITPERDIG); ++i) {
		result.left_shift();
	}
	
	RMLDZRS_L(result.number_);
	return result;


}

LargeInt operator>>(const LargeInt& _a, unsigned int _b) {
	if (vcheck_l(_a.number_) == false) {
		throw invalid_argument("the BasicOp object is illegal");
	}
	LargeInt result = _a;
	if (_b > result.number_[0]*kBITPERDIG)
		return 0;
	auto size = _b / kBITPERDIG;
	for (size_t i = size + 1; i <= result.number_[0]; ++i) {
		result.number_[i - size] = result.number_[i];
	}
	result.number_[0] -= size;
	for (size_t i = 0; i < _b - (size * kBITPERDIG); ++i) {
		result.right_shift();
	}
	return result;
}

LargeInt& LargeInt::operator <<=(unsigned int _a) {
	*this = *this << _a;
	return *this;
}

LargeInt& LargeInt::operator >>=(unsigned int _a) {
	*this = *this >> _a;
	return *this;
}


//_a是被除数，_b是除数。返回余数
LargeInt operator%(const LargeInt& _a, const LargeInt& _b) {
	lint* aptr_l, * bptr_l;
	LINT rem_l, quot_l;//余数与商
	lint* a_ = _a.number_;
	lint* b = _b.number_;
	lint a[2 + (256U << 1)];//kLINTMAXDIGIT
	for (size_t i = 0; i <= DIGITS_L(a_); ++i)
	{
		a[i] = a_[i];
	}
	lint* q_ptr, * b_msd_ptr, * a_lsd_ptr, * a_msd_ptr;
	lint bv, av, qhat, ri, ri_1, ri_2, bn_1, bn_2;
	ULONG right, left, rhat, borrow, carry, sbitsminusd;
	unsigned int d = 0;
	int i;
	if (EQZ_L(b))
		return LargeInt(0);//除数是0，则返回0
	if (EQZ_L(a_)) {
		SETZERO_L(rem_l);
		SETZERO_L(quot_l);
		return LargeInt(rem_l);
	}
	i = cmp_l(a_, b);
	if (i == -1)
	{
		for (size_t i = 0; i <= DIGITS_L(a_); ++i)
		{
			rem_l[i] = a[i];
		}
		SETZERO_L(quot_l);
		return LargeInt(rem_l);
	}
	else if (i == 0)
	{
		SETZERO_L(rem_l);
		SETONE_L(quot_l);
		return LargeInt(rem_l);
	}
	if (DIGITS_L(b) == 1)
	{
		SETZERO_L(rem_l);
		SETZERO_L(quot_l);
		goto shortdiv;
	}
	b_msd_ptr = MSDPTR_L(b);
	bn_1 = *b_msd_ptr;
	while (bn_1 < kBASEDIV2)
	{
		++d;
		bn_1 <<= 1;
	}
	sbitsminusd = (int)(kBITPERDIG - d);
	if (d > 0)
	{
		bn_1 += *(b_msd_ptr - 1) >> sbitsminusd;
		if (DIGITS_L(b) > 2)
		{
			bn_2 = (USHORT)(*(b_msd_ptr - 1) << d) + (*(b_msd_ptr - 2) >> sbitsminusd);
		}
		else
		{
			bn_2 = (USHORT)(*(b_msd_ptr - 1) << d);
		}
	}
	else {
		bn_2 = (USHORT)(*(b_msd_ptr - 1));
	}
	b_msd_ptr = MSDPTR_L(b);
	a_msd_ptr = a + a[0] + 1;
	a_lsd_ptr = a + a[0] - DIGITS_L(b) + 1;
	*(a_msd_ptr) = 0;
	q_ptr = LSDPTR_L(quot_l) + a[0] - DIGITS_L(b);
	while (a_lsd_ptr >= a + 1)
	{
		ri = (USHORT)((*(a_msd_ptr) << d) + (*(a_msd_ptr - 1) >> sbitsminusd));
		ri_1 = (USHORT)((*(a_msd_ptr - 1) << d) + (*(a_msd_ptr - 2) >> sbitsminusd));
		if (a_msd_ptr - 3 > a + 1)
		{
			ri_2 = (USHORT)((*(a_msd_ptr - 2) << d) + (*(a_msd_ptr - 3) >> sbitsminusd));
		}
		else {
			ri_2 = (USHORT)(*(a_msd_ptr - 2) << d);
		}
		if (ri != bn_1)
		{
			qhat = (USHORT)((rhat = ((ULONG)ri << kBITPERDIG) + (ULONG)ri_1) / bn_1);
			right = ((rhat = (rhat - (ULONG)bn_1 * qhat)) << kBITPERDIG) + ri_2;

			if ((left = (ULONG)bn_2 * qhat) > right)
			{
				--qhat;
				if ((rhat + bn_1) < kBASE)
				{
					if ((left - bn_2) > (right + ((ULONG)bn_1 << kBITPERDIG)))
					{
						--qhat;
					}
				}
			}
		}
		else
		{
			qhat = kBASEMINONE;
			right = ((ULONG)(rhat = (ULONG)bn_1 + (ULONG)ri_1) << kBITPERDIG) + ri_2;
			if (rhat < kBASE)
			{
				if ((left = (ULONG)bn_2 * qhat) > right)
				{
					--qhat;
					if ((rhat + bn_1) < kBASE)
					{
						if ((left - bn_2) > (right + ((ULONG)bn_1 << kBITPERDIG)))
						{
							--qhat;
						}
					}
				}

			}
		}
		borrow = kBASE;
		carry = 0;
		for (bptr_l = LSDPTR_L(b), aptr_l = a_lsd_ptr; bptr_l <= b_msd_ptr; ++bptr_l, ++aptr_l)
		{
			if (borrow >= kBASE)
			{
				*aptr_l = (USHORT)(borrow = ((ULONG)*aptr_l + kBASE -
					(ULONG)(USHORT)(carry = (ULONG)*bptr_l *
						qhat + (ULONG)(USHORT)(carry >> kBITPERDIG))));
			}
			else {
				*aptr_l = (USHORT)(borrow = ((ULONG)*aptr_l + kBASEMINONE -
					(ULONG)(USHORT)(carry = (ULONG)*bptr_l *
						qhat + (ULONG)(USHORT)(carry >> kBITPERDIG))));
			}
		}
		if (borrow >= kBASE) {
			*aptr_l = (USHORT)(borrow = ((ULONG)*aptr_l + kBASE - (ULONG)(USHORT)(carry >> kBITPERDIG)));
		}
		else
		{
			*aptr_l = (USHORT)(borrow = ((ULONG)*aptr_l + kBASEMINONE - (ULONG)(USHORT)(carry >> kBITPERDIG)));
		}
		*q_ptr = qhat;
		if (borrow < kBASE)
		{
			carry = 0;
			for (bptr_l = LSDPTR_L(b), aptr_l = a_lsd_ptr; bptr_l <= b_msd_ptr; ++bptr_l, ++aptr_l)
			{
				*aptr_l = (USHORT)(carry = ((ULONG)*aptr_l + (ULONG)(*bptr_l) +
					(ULONG)(USHORT)(carry >> kBITPERDIG)));
			}
			*aptr_l += (USHORT)(carry >> kBITPERDIG);
			--(*q_ptr);
		}
		--a_msd_ptr;
		--a_lsd_ptr;
		--q_ptr;

	}
	SETDIGITS_L(quot_l, a[0] - DIGITS_L(b) + 1);
	RMLDZRS_L(quot_l);
	a[0] = DIGITS_L(b);
	for (size_t i = 0; i <= a[0]; ++i)
	{
		rem_l[i] = a[i];
	}
	RMLDZRS_L(rem_l);
	return LargeInt(rem_l);



shortdiv:
	av = 0;
	bv = *LSDPTR_L(b);
	for (aptr_l = a + a[0], q_ptr = MSDPTR_L(quot_l) + a[0]; aptr_l >= a + 1; aptr_l--, q_ptr--)
	{
		*q_ptr = (USHORT)((rhat = ((((ULONG)av) << kBITPERDIG) + (ULONG)*aptr_l)) / bv);
		av = (USHORT)(rhat - (ULONG)bv * (ULONG)*q_ptr);
	}
	SETDIGITS_L(quot_l, a[0]);
	RMLDZRS_L(quot_l);
	u2lint_l(rem_l, av);
	RMLDZRS_L(rem_l);
	return LargeInt(rem_l);

}

LargeInt operator%(const LargeInt& _a, const lint& _b)
{
	return _a % LargeInt(_b);
}

//_a是被除数，_b是除数。返回商
LargeInt operator/(const LargeInt& _a, const LargeInt& _b) {
	lint* aptr_l, * bptr_l;
	LINT rem_l, quot_l;//余数与商
	lint* a_ = (_a.number_);
	lint* b = (_b.number_);
	lint a[2 + (256U << 1)];//kLINTMAXDIGIT
	for (size_t i = 0; i <= DIGITS_L(a_); ++i)
	{
		a[i] = a_[i];
	}
	lint* q_ptr, * b_msd_ptr, * a_lsd_ptr, * a_msd_ptr;
	lint bv, av, qhat, ri, ri_1, ri_2, bn_1, bn_2;
	ULONG right, left, rhat, borrow, carry, sbitsminusd;
	unsigned int d = 0;
	int i;
	if (EQZ_L(b))
		return LargeInt(0);//除数是0，则返回0
	if (EQZ_L(a_)) {
		SETZERO_L(rem_l);
		SETZERO_L(quot_l);
		return LargeInt(quot_l);
	}
	i = cmp_l(a_, b);
	if (i == -1)
	{
		for (size_t i = 0; i <= DIGITS_L(a_); ++i)
		{
			rem_l[i] = a[i];
		}
		SETZERO_L(quot_l);
		return LargeInt(quot_l);
	}
	else if (i == 0)
	{
		SETZERO_L(rem_l);
		SETONE_L(quot_l);
		return LargeInt(quot_l);
	}
	if (DIGITS_L(b) == 1)
	{
		SETZERO_L(rem_l);
		SETZERO_L(quot_l);
		goto shortdiv;
	}
	b_msd_ptr = MSDPTR_L(b);
	bn_1 = *b_msd_ptr;
	while (bn_1 < kBASEDIV2)
	{
		++d;
		bn_1 <<= 1;
	}
	sbitsminusd = (int)(kBITPERDIG - d);
	if (d > 0)
	{
		bn_1 += *(b_msd_ptr - 1) >> sbitsminusd;
		if (DIGITS_L(b) > 2)
		{
			bn_2 = (lint)(*(b_msd_ptr - 1) << d) + (*(b_msd_ptr - 2) >> sbitsminusd);
		}
		else
		{
			bn_2 = (lint)(*(b_msd_ptr - 1) << d);
		}
	}
	else {
		bn_2 = (lint)(*(b_msd_ptr - 1));
	}
	b_msd_ptr = MSDPTR_L(b);
	a_msd_ptr = a + a[0] + 1;
	a_lsd_ptr = a + a[0] - DIGITS_L(b) + 1;
	*(a_msd_ptr) = 0;
	q_ptr = LSDPTR_L(quot_l) + a[0] - DIGITS_L(b);
	while (a_lsd_ptr >= a + 1)
	{
		ri = (USHORT)((*(a_msd_ptr) << d) + (*(a_msd_ptr - 1) >> sbitsminusd));
		ri_1 = (USHORT)((*(a_msd_ptr - 1) << d) + (*(a_msd_ptr - 2) >> sbitsminusd));
		if (a_msd_ptr - 3 > a + 1)
		{
			ri_2 = (USHORT)((*(a_msd_ptr - 2) << d) + (*(a_msd_ptr - 3) >> sbitsminusd));
		}
		else {
			ri_2 = (USHORT)(*(a_msd_ptr - 2) << d);
		}
		if (ri != bn_1)
		{
			qhat = (USHORT)((rhat = ((ULONG)ri << kBITPERDIG) + (ULONG)ri_1) / bn_1);
			right = ((rhat = (rhat - (ULONG)bn_1 * qhat)) << kBITPERDIG) + ri_2;

			if ((left = (ULONG)bn_2 * qhat) > right)
			{
				--qhat;
				if ((rhat + bn_1) < kBASE)
				{
					if ((left - bn_2) > (right + ((ULONG)bn_1 << kBITPERDIG)))
					{
						--qhat;
					}
				}
			}
		}
		else
		{
			qhat = kBASEMINONE;
			right = ((ULONG)(rhat = (ULONG)bn_1 + (ULONG)ri_1) << kBITPERDIG) + ri_2;
			if (rhat < kBASE)
			{
				if ((left = (ULONG)bn_2 * qhat) > right)
				{
					--qhat;
					if ((rhat + bn_1) < kBASE)
					{
						if ((left - bn_2) > (right + ((ULONG)bn_1 << kBITPERDIG)))
						{
							--qhat;
						}
					}
				}

			}
		}
		borrow = kBASE;
		carry = 0;
		for (bptr_l = LSDPTR_L(b), aptr_l = a_lsd_ptr; bptr_l <= b_msd_ptr; bptr_l++, aptr_l++)
		{
			if (borrow >= kBASE)
			{
				*aptr_l = (USHORT)(borrow = ((ULONG)*aptr_l + kBASE -
					(ULONG)(USHORT)(carry = (ULONG)*bptr_l *
						qhat + (ULONG)(USHORT)(carry >> kBITPERDIG))));
			}
			else {
				*aptr_l = (USHORT)(borrow = ((ULONG)*aptr_l + kBASEMINONE -
					(ULONG)(USHORT)(carry = (ULONG)*bptr_l *
						qhat + (ULONG)(USHORT)(carry >> kBITPERDIG))));
			}
		}
		if (borrow >= kBASE) {
			*aptr_l = (USHORT)(borrow = ((ULONG)*aptr_l + kBASE - (ULONG)(USHORT)(carry >> kBITPERDIG)));
		}
		else
		{
			*aptr_l = (USHORT)(borrow = ((ULONG)*aptr_l + kBASEMINONE - (ULONG)(USHORT)(carry >> kBITPERDIG)));
		}
		*q_ptr = qhat;
		if (borrow < kBASE)
		{
			carry = 0;
			for (bptr_l = LSDPTR_L(b), aptr_l = a_lsd_ptr; bptr_l <= b_msd_ptr; bptr_l++, aptr_l++)
			{
				*aptr_l = (USHORT)(carry = ((ULONG)*aptr_l + (ULONG)(*bptr_l) +
					(ULONG)(USHORT)(carry >> kBITPERDIG)));
			}
			*aptr_l += (USHORT)(carry >> kBITPERDIG);
			--(*q_ptr);
		}
		--a_msd_ptr;
		--a_lsd_ptr;
		--q_ptr;

	}
	SETDIGITS_L(quot_l, a[0] - DIGITS_L(b) + 1);
	RMLDZRS_L(quot_l);
	a[0] = DIGITS_L(b);
	for (size_t i = 0; i <= a[0]; ++i)
	{
		rem_l[i] = a[i];
	}
	return LargeInt(quot_l);



shortdiv:
	av = 0;
	bv = *LSDPTR_L(b);
	for (aptr_l = a + a[0], q_ptr = MSDPTR_L(quot_l) + a[0]; aptr_l >= a + 1; --aptr_l, --q_ptr)
	{
		*q_ptr = (USHORT)((rhat = ((((ULONG)av) << kBITPERDIG) + (ULONG)*aptr_l)) / bv);
		av = (USHORT)(rhat - (ULONG)bv * (ULONG)*q_ptr);
	}
	SETDIGITS_L(quot_l, a[0]);
	RMLDZRS_L(quot_l);
	u2lint_l(rem_l, av);
	return LargeInt(quot_l);

}

LargeInt operator/(const LargeInt& _a, const lint& _b)
{
	return _a / LargeInt(_b);
}


LargeInt& LargeInt::operator %=(const LargeInt& _a) {
	*this = *this % _a;
	return *this;
}

LargeInt& LargeInt::operator %=(lint _a) {
	*this = *this % _a;
	return *this;
}

LargeInt& LargeInt::operator /=(const LargeInt& _a) {
	*this = *this / _a;
	return *this;
}

LargeInt& LargeInt::operator /=(lint _a) {
	*this = *this / _a;
	return *this;
}

bool LargeInt::iseven() {
	return (ISEVEN_L(number_));
}

bool LargeInt::isodd() {
	return (ISODD_L(number_));
}

string LargeInt::hex_str(){
	if (number_[0] == 0) {
		return "0";
	}
	else {
		stringstream result;
		for (lint i = number_[0]; i > 0; i--) {
			if (i != number_[0])
				result << uppercase << hex << setfill('0') << setw(8) << number_[i];
			else
				result << uppercase << hex << number_[i];
		}
		return result.str();
	}
	
}

lint* LargeInt::value() {
	return number_;
}

const lint* LargeInt::value()const {
	return number_;
}

lint* LargeInt::copyvalue()
{
	lint* a = new LINT;
	for (int i = 0; i <= DIGITS_L(number_); ++i)
		a[i] = number_[i];
	return a;
}

lint* LargeInt::copyvalue() const {
	lint* a = new LINT;
	for (int i = 0; i <= DIGITS_L(number_); ++i)
		a[i] = number_[i];
	return a;
}

unsigned int LargeInt::id_l()
{
	int i = 0;
	lint tmp;
	while (*(number_ + i) != 0xcdcdcdcd)
	{
		i++;
		tmp = *(number_ + i);
	}
	i = i - 2;
	lint a = *(number_ + i + 1);
	int count = 0;//定义一个计数器变量，a%2==1成立count++
	while (a) {
		a = a >> 1;
		count++;

	}//只有当a/2不为0是其结束条件
	return count + i * 16;
}


ostream& operator<<(ostream& _os, const LargeInt& _a) {
	_os << LINT2str_l(_a.number_) << flush;
	return _os;
}



