#include "number_support.h"
#include<iostream>
#include<iomanip>
#include<string>
#include<sstream>
#include<stdexcept>
#include<cctype>
#include<algorithm>
#include<iterator>
using namespace std;
//错误代码
extern const short kE_LINT_OK = 0;//OK
extern const short kE_LINT_DBZ = -1;//被零除
extern const short kE_LINT_OFL = -2;//上溢
extern const short kE_LINT_UFL = -3;//下溢
extern const short kE_LINT_MAL = -4;//内存分配错误
extern const short kE_LINT_NOR = -5;//寄存器不可用
extern const short kE_LINT_BOR = -6;//基数不合法
extern const short kE_LINT_MOD = -7;//模数为偶
extern const short kE_LINT_NPT = -8;//将空指针作为参数传递
extern const short kE_VCHECK_OFL = 1;//警告：数字过长
extern const short kE_VCHECK_LDZ = 2;//警告：前导零
extern const short kE_VCHECK_MEM = -1;//空指针

//常数
extern const unsigned long long kBASE = 0X100000000;//基数B
extern const unsigned long long kBASEMINONE = 0XFFFFFFFF;//B - 1
extern const unsigned long long kDBASEMINONE = 0XFFFFFFFFFFFFFFFF;//B^2 - 1
extern const unsigned long kBASEDIV2 = 0X80000000U;//B/2
extern const unsigned int kNOOFREGS = 16U;//寄存器标准编号
extern const unsigned int kBITPERDIG = 32U;//每个LINT数字中包含的二进制位数
extern const unsigned int kLDBITPERDGT = 5U;//以二为底kBITPERDIG的对数
extern const unsigned int kLINTMAXDIGIT = 128U;//LINT对象以B为基数的最大位数
extern const unsigned int kLINTMAXSHORT = kLINTMAXDIGIT + 1;//为LINT对象分配的USHORT个数
extern const unsigned int kLINTMAXBYTE = kLINTMAXSHORT << 2;//为LINT对象分配的字节数
extern const unsigned int kLINTMAXBIT = kLINTMAXDIGIT << 5;//LINT对象包含的最大二进制位数
extern const double kFLINT_VERSION = 0.1;//版本号
extern const std::pair<unsigned int, unsigned int> kLINT_SECURE = { 0X73,0 };//安全模式标识符


//函数

//检查n_l_是否是偶数
bool ISEVEN_L(const lint* n_) {
	return (DIGITS_L(n_) == 0) || ((DIGITS_L(n_) > 0) && ((*(LSDPTR_L(n_)) & 1) == 0));
}
//检查n_l_是否是奇数
bool ISODD_L(const lint* n_) {
	return (DIGITS_L(n_) > 0) && (((*(LSDPTR_L(n_))) & 1) == 1);
}
//模Nmax+1
void ANDMAX_L(lint* n_) {
	SETDIGITS_L(n_, std::min(DIGITS_L(n_), (lint)kLINTMAXDIGIT));
	RMLDZRS_L(n_);
}

//比较两个LINT对象是否相等
bool equ_l(const lint* a_, const lint* b_) {
	const lint* msdptr_a, * msdptr_b;
	lint la = DIGITS_L(a_);
	lint lb = DIGITS_L(b_);
	if (la == 0 && lb == 0)
		return true;
	while ((a_[la] == 0) && (la > 0)) {
		--la;
	}
	while ((b_[lb] == 0) && (lb > 0)) {
		--lb;
	}
	if (la != lb)
		return false;
	if ((la == 0) && (lb == 0))
		return true;
	msdptr_a = a_ + la;
	msdptr_b = b_ + lb;
	while ((*msdptr_a == *msdptr_b) && (msdptr_a > a_)) {
		--msdptr_a;
		--msdptr_b;
	}
	return (msdptr_a > a_ ? false : true);
}
//比较两个LINT对象(a_>b_->1; a_<b_->-1; a_==b_->0)
int cmp_l(const lint* a_, const lint* b_) {
	const lint* msdptr_a, * msdptr_b;
	lint la = DIGITS_L(a_);
	lint lb = DIGITS_L(b_);
	if (la == 0 && lb == 0)
		return 0;
	while (a_[la] == 0 && la > 0) {
		--la;
	}
	while (b_[lb] == 0 && lb > 0) {
		--lb;
	}
	if (la > lb)
		return 1;
	if (la < lb)
		return -1;
	if ((la == 0) && (lb == 0))
		return 0;


	msdptr_a = a_ + la;
	msdptr_b = b_ + lb;
	while ((*msdptr_a == *msdptr_b) && (msdptr_a > a_)) {
		--msdptr_a;
		--msdptr_b;
	}
	if (msdptr_a == a_)
		return 0;
	if (*msdptr_a > *msdptr_b)
		return 1;
	return -1;
}
//复制一个LINT对象
void cpy_l(lint* dest_, const lint* src_) {
	for (size_t i = 0; i <= src_[0]; ++i) {
		dest_[i] = src_[i];
	}
	RMLDZRS_L(dest_);
}

//将一个unsigned long 类型的数字转换为LINT类型对象
void u2lint_l(lint* num_, USHORT u) {
	num_[1] = u;
	SETDIGITS_L(num_, 1);
}
//比较a_>=b_
bool GE_L(const lint* a_, const lint* b_) {
	return (cmp_l(a_, b_) > -1);
}
//比较a_>b_
bool GT_L(const lint* a_, const lint* b_) {
	return (cmp_l(a_, b_) == 1);
}
//比较a_<b_
bool LT_L(const lint* a_, const lint* b_) {
	return (cmp_l(a_, b_) == -1);
}

//比较a_==0
bool EQZ_L(const lint* a_) {
	LINT tmp;
	u2lint_l(tmp, 0);
	return (equ_l(a_, tmp));
}
//比较a_==1
bool EQONE_L(const lint* a_) {
	LINT tmp;
	u2lint_l(tmp, 1);
	return (equ_l(a_, tmp));
}
//比较a_>0
bool GTZ_L(const lint* a_) {
	LINT tmp;
	u2lint_l(tmp, 0);
	return (GT_L(a_, tmp));
}

//将a_设为0
void SETZERO_L(lint* a_) {
	a_[0] = 0;
}
//将a_设为1
void SETONE_L(lint* a_) {
	u2lint_l(a_, 1);
}
//将a_设为2
void SETTWO_L(lint* a_) {
	u2lint_l(a_, 2);
}
//将a_设为ul
void SET_L(lint* a_, USHORT ul) {
	u2lint_l(a_, ul);
}
//将a_设置为Nmax
void SETMAX_L(lint* a_) {
	SETDIGITS_L(a_, kLINTMAXDIGIT);
	for (size_t i = 1; i <= kLINTMAXDIGIT; ++i) {
		a_[i] = ULONG_MAX;
	}
}
//将LINT转为十六进制字符串
string LINT2str_l(const lint* a_) {
	ostringstream result;
	if (a_[0] == 0) {
		result << '0';
	}
	else {
		for (lint i = a_[0]; i > 0; i--) {
			if(i != a_[0])
				result << uppercase << hex << setfill('0') << setw(8) << a_[i];
			else 
				result << uppercase << hex << a_[i];
		}
	}
	return result.str();
}
//将一个字符串以16进制转为LINT
void str2LINT_l(lint* num_, const string& s_) {
	SETZERO_L(num_);
	try {
		if (s_.empty())
			throw invalid_argument("s_ is empty");
	}
	catch (invalid_argument e) {
		cerr << e.what();
		return;
	}

	string s = s_;
	if (s_[0] == '0') {
		if (s_[1] == 'x' || s_[1] == 'X')
			s = s_.substr(2);
	}
	ostringstream os;
	os << setfill('0') << setw(ceil((double)s.size() / 8) * 8) << s;
	s = os.str();
	lint size = (lint)(s.size() / 8);
	num_[0] = size;
	for (size_t i = size; i > 0; i--) {
		size_t j = size - i;
		num_[i] = (lint)stoll(s.substr(8 * j, 8), nullptr, 16);
	}

}

