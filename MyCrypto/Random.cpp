#include "Random.h"
#include"Mod.h"
#include<random>
#include<sstream>
using namespace std;
static const size_t prime[] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71\
,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173\
,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281\
,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409\
,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541\
,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659\
,661,673,677,683	,691	,701	,709,719,	727,	733,	739,	743,	751,	757,	761,	769,	773,	787,	797,	809,\
811,	821,	823,	827,	829,	839,	853,	857,	859,	863,	877,	881,	883,	887,	907,	911,	919,	929,	937,	941,\
947,	953,	967,	971,	977,	983,	991,	997,	1009,	1013,	1019,	1021,	1031,	1033,	1039,	1049,	1051,	1061,	1063,	1069,\
1087,	1091,	1093,	1097,	1103,	1109,	1117,	1123,	1129,	1151,	1153,	1163,	1171,	1181,	1187,	1193,	1201,	1213,	1217,	1223,\
1229,	1231,	1237,	1249,	1259,	1277,	1279,	1283,	1289,	1291,	1297,	1301,	1303,	1307,	1319,	1321,	1327,	1361,	1367,	1373,\
1381,	1399,	1409,	1423,	1427,	1429,	1433,	1439,	1447,	1451,	1453,	1459,	1471,	1481,	1483,	1487,	1489,	1493,	1499,	1511,\
1523,	1531,	1543,	1549,	1553,	1559,	1567,	1571,	1579,	1583,	1597,	1601,	1607,	1609,	1613,	1619,	1621,	1627,	1637,	1657,\
1663,	1667,	1669,	1693,	1697,	1699,	1709,	1721,	1723,	1733,	1741,	1747,	1753,	1759,	1777,	1783,	1787,	1789,	1801,	1811,\
1823,	1831,	1847,	1861,	1867,	1871,	1873,	1877,	1879,	1889,	1901,	1907,	1913,	1931,	1933,	1949,	1951,	1973,	1979,	1987,\
1993,	1997,	1999 };

string get_random_bytes(size_t N) {
    char tmp;							// tmp: 暂存一个随机数
    string buffer;						// buffer: 保存返回值

    // 下面这两行比较重要:
    random_device rd;					// 产生一个 std::random_device 对象 rd
    default_random_engine random(rd());	// 用 rd 初始化一个随机数发生器 random

    for (int i = 0; i < N; i++) {
        tmp = random() % 36;	// 随机一个小于 36 的整数，0-9、A-Z 共 36 种字符
        if (tmp < 10) {			// 如果随机数小于 10，变换成一个阿拉伯数字的 ASCII
            tmp += '0';
        }
        else {				// 否则，变换成一个大写字母的 ASCII
            tmp -= 10;
            tmp += 'A';
        }
        buffer += tmp;
    }
    return buffer;
}

LargeInt get_random_bits(size_t N) {
    stringstream res;
    static uniform_int_distribution<unsigned>u(0, 15);
    static default_random_engine e(unsigned int(time(0)));
    for (size_t i = 0; i < N/4; ++i) {
        res << hex << u(e);
    }
    return LargeInt(res.str());
}


//生成n比特伪素数
LargeInt pseudo_prime(size_t N) {
    LargeInt res;
    bool is_prime = false;
    int i;
    while (1) {
        res = get_random_bits(N);
        //保证res是奇数
        if ((res & 1) == 0)
            ++res;
        is_prime = true;
        for (i = 0; i < 302; ++i) {
            if (res == prime[i])
                return res;
            if (res % prime[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime == true)
            return res;
        
    }
    return res;
}

//Miller-Rabin素数检测
bool miller_rabin(LargeInt n)
{
    LargeInt k = 0;
    LargeInt p = n - 1;
    while ((p & 1) == 0) {
        p = p >> 1;
        k++;
    }
    for (int i = 0; i < 6; ++i) {
        LargeInt a = rand() % (n - 1 - 1 + 1) + 1;
        LargeInt b = LargeInt_pow(a, p, n);
        bool flag = false;
        if (b == 1)
            continue;
        for (LargeInt j = 0; j < k; ++j) {
            if ((b + 1) % n == 0) {
                flag = true;
                break;
            }
            else
                b = (b * b) % n;
        }
        if (flag)
            continue;
        return false;
    }
    return true;
  
}



LargeInt get_random_prime(size_t N) {
    LargeInt prime;
    while (1) {
        prime = pseudo_prime(N);
        if (miller_rabin(prime) == true)
            return prime;
    }
}