#pragma once
#include<string>
#include"LargeInt.h"
using std::string;
//随机生成一个长度为N的字符串
string get_random_bytes(size_t N);
//生成一个最多N比特的大整数(N=0mod4)
LargeInt get_random_bits(size_t N);
//随机生成一个N比特的大素数
LargeInt get_random_prime(size_t N);

