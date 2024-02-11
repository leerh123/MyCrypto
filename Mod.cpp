#include "Mod.h"
#include<iostream>
using namespace std;

LargeInt LargeInt_pow(LargeInt x, LargeInt n) {
	LargeInt res = 1;
	while (n > 0) {
		if ((n & 1) != 0)
			res = (res * x);
		x = (x * x);
		n >>= 1;
	}
	return res;
}
LargeInt LargeInt_pow(LargeInt x, LargeInt n, LargeInt m) {
    LargeInt res = 1;
    while (n > 0) {
        if ((n & 1) != 0)
            res = (res * x) % m;
        x = (x * x) % m;
        n >>= 1;
    }
    return res;
}


LargeInt inverse(LargeInt a_l, LargeInt n_l) {
	if ((a_l == 0) || (n_l == 0))
		return 0;
	LargeInt g_l = a_l;
	LargeInt v3_l = n_l;
	LargeInt v1_l = 0;
	LargeInt t1_l = 1;
	LargeInt q_l, t3_l;
	do {
		q_l = g_l / v3_l, t3_l = g_l % v3_l;
		if (t3_l > 0) {
			q_l = (v1_l * q_l) % n_l;
			q_l = (n_l + t1_l - q_l) % n_l;
			t1_l = v1_l;
			v1_l = q_l;
			g_l = v3_l;
			v3_l = t3_l;
		}
	} while (t3_l > 0);
	g_l = v3_l;
	if (g_l == 1)
		return v1_l;
	else
		return 0;
}

LargeInt gcd(const LargeInt& _a, const LargeInt& _b) {
	LargeInt longer = _a;
	LargeInt shorter = _b;
	unsigned int k = 0;
	if (_a < _b) {
		swap(longer, shorter);
	}
	if (shorter == 0) {
		return longer;
	}

	LargeInt tmp = longer % shorter;
	longer = shorter;
	shorter = tmp;

	if (shorter == 0) {
		return longer;
	}
	while (longer.iseven() && shorter.iseven()) {
		++k;
		shorter >>= 1;
		longer >>= 1;
	}
	while (shorter.iseven()) {
		shorter >>= 1;
	}
	while (longer.iseven()) {
		longer >>= 1;
	}
	while (1) {
		int sign_of_t = 1;
		if (longer < shorter) {
			tmp = shorter - longer;
			sign_of_t = -1;
		}
		else {
			tmp = longer - shorter;
		}
		if (*(tmp.value()) == 0) {
			return (longer << k);
		}
		while (tmp.iseven()) {
			tmp >>= 1;
		}
		if (sign_of_t == -1) {
			shorter = tmp;
		}
		else {
			longer = tmp;
		}
	}
}