#pragma once
#include<utility>
#include<array>
#include<algorithm>
#include<string>


//�������
extern const short kE_LINT_OK;//OK
extern const short kE_LINT_DBZ;//�����
extern const short kE_LINT_OFL;//����
extern const short kE_LINT_UFL;//����
extern const short kE_LINT_MAL;//�ڴ�������
extern const short kE_LINT_NOR;//�Ĵ���������
extern const short kE_LINT_BOR;//�������Ϸ�
extern const short kE_LINT_MOD;//ģ��Ϊż
extern const short kE_LINT_NPT;//����ָ����Ϊ��������
extern const short kE_VCHECK_OFL;//���棺���ֹ���
extern const short kE_VCHECK_LDZ;//���棺ǰ����
extern const short kE_VCHECK_MEM;//��ָ��

//����
extern const unsigned long long kBASE;//����B
extern const unsigned long long kBASEMINONE;//B - 1
extern const unsigned long long kDBASEMINONE;//B^2 - 1
extern const unsigned long kBASEDIV2;//B/2
extern const unsigned int kNOOFREGS;//�Ĵ�����׼���
extern const unsigned int kBITPERDIG;//ÿ��LINT�����а����Ķ�����λ��
extern const unsigned int kLDBITPERDGT;//�Զ�Ϊ��kBITPERDIG�Ķ���
extern const unsigned int kLINTMAXDIGIT;//LINT������BΪ���������λ��
extern const unsigned int kLINTMAXSHORT;//ΪLINT��������USHORT����
extern const unsigned int kLINTMAXBYTE;//ΪLINT���������ֽ���
extern const unsigned int kLINTMAXBIT;//LINT�����������������λ��
extern const double kFLINT_VERSION;//�汾��
extern const std::pair<unsigned int, unsigned int> kLINT_SECURE;//��ȫģʽ��ʶ��

//����
using USHORT = unsigned long;
using ULONG = unsigned long long;
using lint = USHORT;
using LINT = lint[129];//kLINTMAXDIGIT + 1
using LINTD = lint[257];//(kLINTMAXDIGIT << 1) + 1
using LINTQ = lint[513];//(kLINTMAXDIGIT << 2) + 1

//����
//��n_l_��λ������Ϊa_
inline void SETDIGITS_L(lint* n_, lint a_) {
	n_[0] = a_;
}

//��ȡn_l_��λ��
inline lint DIGITS_L(const lint* n_) {
	return n_[0];
}

//��ȡ�����Чλ��ָ��
inline lint* LSDPTR_L(lint* n_) {
	return n_ + 1;
}
inline const lint* LSDPTR_L(const lint* n_) {
	return n_ + 1;
}

//��ȡ�����Чλ��ָ��
inline lint* MSDPTR_L(lint* n_) {
	return n_ + DIGITS_L(n_);
}
inline const lint* MSDPTR_L(const lint* n_) {
	return n_ + DIGITS_L(n_);
}

//λ����һ
inline void DECDIGITS_L(lint* n_) {
	if (n_[0] == 0)
		return;
	--n_[0];
}

//λ����һ
inline void INCDIGITS_L(lint* n_) {
	++n_[0];
}

//ȥ��ǰ����
inline void RMLDZRS_L(lint* n_) {
	while ((DIGITS_L(n_) > 0) && (*(MSDPTR_L(n_)) == 0))
		DECDIGITS_L(n_);
}

//����ʽ�Ƿ�Ϸ�
inline bool vcheck_l(const lint* n_) {
	//����
	if (DIGITS_L(n_) > kLINTMAXDIGIT)
		return false;
	return true;
}

//���n_l_�Ƿ���ż��
bool ISEVEN_L(const lint* n_);
//���n_l_�Ƿ�������
bool ISODD_L(const lint* n_);
//ģNmax+1
void ANDMAX_L(lint* n_);
//�Ƚ�����LINT�����Ƿ����
bool equ_l(const lint* a_, const lint* b_);
//�Ƚ�����LINT����(a_>b_->1; a_<b_->-1; a_==b_->0)
int cmp_l(const lint* a_, const lint* b_);
//����һ��LINT����
void cpy_l(lint* dest_, const lint* src_);
//��һ��unsigned long ���͵�����ת��ΪLINT���Ͷ���
void u2lint_l(lint* num_, USHORT u);
//�Ƚ�a_>=b_
bool GE_L(const lint* a_, const lint* b_);
//�Ƚ�a_>b_
bool GT_L(const lint* a_, const lint* b_);
//�Ƚ�a_<b_
bool LT_L(const lint* a_, const lint* b_);
//�Ƚ�a_==0
bool EQZ_L(const lint* a_);
//�Ƚ�a_==1
bool EQONE_L(const lint* a_);
//�Ƚ�a_>0
bool GTZ_L(const lint* a_);
//��a_��Ϊ0
void SETZERO_L(lint* a_);
//��a_��Ϊ1
void SETONE_L(lint* a_);
//��a_��Ϊ2
void SETTWO_L(lint* a_);
//��a_��Ϊul
void SET_L(lint* a_, unsigned long ul);
//��a_����ΪNmax
void SETMAX_L(lint* a_);
//��LINTתΪʮ�������ַ���
std::string LINT2str_l(const lint* a_);
//��һ���ַ�����16����תΪLINT
void str2LINT_l(lint* num_, const std::string& s_);



