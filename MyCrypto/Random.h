#pragma once
#include<string>
#include"LargeInt.h"
using std::string;
//�������һ������ΪN���ַ���
string get_random_bytes(size_t N);
//����һ�����N���صĴ�����(N=0mod4)
LargeInt get_random_bits(size_t N);
//�������һ��N���صĴ�����
LargeInt get_random_prime(size_t N);

