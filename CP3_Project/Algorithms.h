#pragma once

#include <math.h>

namespace Application
{
	unsigned int Gcd(int a, int b)
	{
		a = abs(a);
		b = abs(b);
		if (a < b) return Gcd(b, a);
		while (b>0)
		{
			unsigned mod = a%b;
			a = b;
			b = mod;
		}
		return a;
	}
	int Min(const int& a, const int& b)
	{
		return a < b ? a : b;
	}
}