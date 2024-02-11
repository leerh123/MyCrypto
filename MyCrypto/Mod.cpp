#include "Mod.h"
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


// �����汾����չŷ������㷨  
LargeInt extended_gcd(LargeInt a, LargeInt b, LargeInt& x, LargeInt& y) {
	if (a == 0) {
		x = 0;
		y = 1;
		return b;
	}
	LargeInt x1 = 1, y1 = 0;
	LargeInt x0 = 0, y0 = 1;
	LargeInt r = a % b;
	LargeInt q = a / b;

	while (r != 0) {
		LargeInt x2 = x0 - q * x1;
		LargeInt y2 = y0 - q * y1;
		x0 = x1;
		y0 = y1;
		x1 = x2;
		y1 = y2;
		a = b;
		b = r;
		r = a % b;
		q = a / b;
	}

	x = x1;
	y = y1;
	return b;
}

// ��ģ��Ԫ  
LargeInt inverse(LargeInt a, LargeInt m) {
	LargeInt x, y;
	LargeInt gcd = extended_gcd(a, m, x, y);
	if (gcd != 1) {
		return -1; // a �� m �����ʣ�û����Ԫ  
	}
	return (x % m + m) % m; // ȷ������� [0, m-1] ��Χ��  
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