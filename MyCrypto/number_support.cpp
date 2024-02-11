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
//�������
extern const short kE_LINT_OK = 0;//OK
extern const short kE_LINT_DBZ = -1;//�����
extern const short kE_LINT_OFL = -2;//����
extern const short kE_LINT_UFL = -3;//����
extern const short kE_LINT_MAL = -4;//�ڴ�������
extern const short kE_LINT_NOR = -5;//�Ĵ���������
extern const short kE_LINT_BOR = -6;//�������Ϸ�
extern const short kE_LINT_MOD = -7;//ģ��Ϊż
extern const short kE_LINT_NPT = -8;//����ָ����Ϊ��������
extern const short kE_VCHECK_OFL = 1;//���棺���ֹ���
extern const short kE_VCHECK_LDZ = 2;//���棺ǰ����
extern const short kE_VCHECK_MEM = -1;//��ָ��

//����
extern const unsigned long long kBASE = 0X100000000;//����B
extern const unsigned long long kBASEMINONE = 0XFFFFFFFF;//B - 1
extern const unsigned long long kDBASEMINONE = 0XFFFFFFFFFFFFFFFF;//B^2 - 1
extern const unsigned long kBASEDIV2 = 0X80000000U;//B/2
extern const unsigned int kNOOFREGS = 16U;//�Ĵ�����׼���
extern const unsigned int kBITPERDIG = 32U;//ÿ��LINT�����а����Ķ�����λ��
extern const unsigned int kLDBITPERDGT = 5U;//�Զ�Ϊ��kBITPERDIG�Ķ���
extern const unsigned int kLINTMAXDIGIT = 128U;//LINT������BΪ���������λ��
extern const unsigned int kLINTMAXSHORT = kLINTMAXDIGIT + 1;//ΪLINT��������USHORT����
extern const unsigned int kLINTMAXBYTE = kLINTMAXSHORT << 2;//ΪLINT���������ֽ���
extern const unsigned int kLINTMAXBIT = kLINTMAXDIGIT << 5;//LINT�����������������λ��
extern const double kFLINT_VERSION = 0.1;//�汾��
extern const std::pair<unsigned int, unsigned int> kLINT_SECURE = { 0X73,0 };//��ȫģʽ��ʶ��


//����

//���n_l_�Ƿ���ż��
bool ISEVEN_L(const lint* n_) {
	return (DIGITS_L(n_) == 0) || ((DIGITS_L(n_) > 0) && ((*(LSDPTR_L(n_)) & 1) == 0));
}
//���n_l_�Ƿ�������
bool ISODD_L(const lint* n_) {
	return (DIGITS_L(n_) > 0) && (((*(LSDPTR_L(n_))) & 1) == 1);
}
//ģNmax+1
void ANDMAX_L(lint* n_) {
	SETDIGITS_L(n_, std::min(DIGITS_L(n_), (lint)kLINTMAXDIGIT));
	RMLDZRS_L(n_);
}

//�Ƚ�����LINT�����Ƿ����
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
//�Ƚ�����LINT����(a_>b_->1; a_<b_->-1; a_==b_->0)
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
//����һ��LINT����
void cpy_l(lint* dest_, const lint* src_) {
	for (size_t i = 0; i <= src_[0]; ++i) {
		dest_[i] = src_[i];
	}
	RMLDZRS_L(dest_);
}

//��һ��unsigned long ���͵�����ת��ΪLINT���Ͷ���
void u2lint_l(lint* num_, USHORT u) {
	num_[1] = u;
	SETDIGITS_L(num_, 1);
}
//�Ƚ�a_>=b_
bool GE_L(const lint* a_, const lint* b_) {
	return (cmp_l(a_, b_) > -1);
}
//�Ƚ�a_>b_
bool GT_L(const lint* a_, const lint* b_) {
	return (cmp_l(a_, b_) == 1);
}
//�Ƚ�a_<b_
bool LT_L(const lint* a_, const lint* b_) {
	return (cmp_l(a_, b_) == -1);
}

//�Ƚ�a_==0
bool EQZ_L(const lint* a_) {
	LINT tmp;
	u2lint_l(tmp, 0);
	return (equ_l(a_, tmp));
}
//�Ƚ�a_==1
bool EQONE_L(const lint* a_) {
	LINT tmp;
	u2lint_l(tmp, 1);
	return (equ_l(a_, tmp));
}
//�Ƚ�a_>0
bool GTZ_L(const lint* a_) {
	LINT tmp;
	u2lint_l(tmp, 0);
	return (GT_L(a_, tmp));
}

//��a_��Ϊ0
void SETZERO_L(lint* a_) {
	a_[0] = 0;
}
//��a_��Ϊ1
void SETONE_L(lint* a_) {
	u2lint_l(a_, 1);
}
//��a_��Ϊ2
void SETTWO_L(lint* a_) {
	u2lint_l(a_, 2);
}
//��a_��Ϊul
void SET_L(lint* a_, USHORT ul) {
	u2lint_l(a_, ul);
}
//��a_����ΪNmax
void SETMAX_L(lint* a_) {
	SETDIGITS_L(a_, kLINTMAXDIGIT);
	for (size_t i = 1; i <= kLINTMAXDIGIT; ++i) {
		a_[i] = ULONG_MAX;
	}
}
//��LINTתΪʮ�������ַ���
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
//��һ���ַ�����16����תΪLINT
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

