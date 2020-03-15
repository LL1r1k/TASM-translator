#pragma once
#include <string>

std::string i2b(int a, int lenght = 0)
{
	if (!lenght)
		lenght = ceil(log2(a));
	std::string res(lenght, '0');
	int w = 1;
	for (int i = lenght - 1; i >= 0; i--, w <<= 1)
		if (a & w)
			res[i] = '1';
	return res;
}

std::string i2h(int a, int lenght)
{
	const char* digits = "0123456789ABCDEF";
	std::string res(lenght, '0');
	for (int i = 0, j = (lenght - 1) * 4; i < lenght; ++i, j -= 4)
		res[i] = digits[(a >> j) & 0x0f];
	return res;
}

std::string h2b(std::string hex, int lenght)
{
	const char* digits = "0123456789ABCDEF";
	std::string res(lenght - hex.length() * 4, '0');
	for (int i = 0; i < hex.length(); i++)
	{
		if (hex[i] >= 'a' )
			res += i2b(hex[i] - 'a' + 10, 4);
		else
			res += i2b(hex[i] - '0', 4);
	}
	return res;
}
