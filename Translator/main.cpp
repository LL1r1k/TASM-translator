#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "tobin.h"
#include "reg.h"

using namespace std;
enum eOP
{
	DEC_R, DEC_RM, XCHG_AX, XCHG_R
};

string opDec = "dec", opXchg = "xchg";
string op[] = { "01001", "1111111", "10010", "1000011"}; 
map<string, int> rm = { {"bx", 7 }, {"bp", 6} };

string modregrm(int treg, string& operand, bool with_disp, int& add)
{
	string res = "";
	if (operand.back() != ']') // mod == 11 reg == r/m
	{
		add++;
		res += "11";
		res += i2b(treg, 3);
		res += i2b(reg[operand], 3);
	}
	else
	{
		// [bx], [bp], [bx + disp], [bp + disp]
		if (with_disp) // mod == 01 || 10
		{
			bool mod = false;
			string disp = operand.substr(4);
			disp.erase(disp.length() - 1, 1);
			if (disp.back() == 'h')
			{
				disp.erase(disp.length() - 1, 1);
				if(disp[0] == '0')
					disp.erase(0, 1);
				string tmpd;
				if (disp.length() > 2)
				{
					tmpd = disp.substr(disp.length() - 2, 2);
					tmpd = h2b(tmpd, 8);
					disp = disp.substr(0, disp.length()/2);
					tmpd += " | " + h2b(disp, 8);;
					disp = tmpd;
					mod = true;
				}
				else
				{
					disp = h2b(disp, 8);
					if (disp.front() == '1')
					{
						mod = true;
						disp += " | " + string(8, '0');
					}
				}
			}
			else if (disp.back() != 'b')
			{
				int d = atoi(disp.c_str());
				int tmpd = d >> 8;
				disp = i2b(d, 8);
				if (tmpd != 0)
				{
					disp += " | " + i2b(tmpd, 8);
					mod = true;
				}		
			}
			else
			{
				disp.erase(disp.length() - 1, 1);
				if (disp.length() <= 8)
				{
					string tmp(8 - disp.length(), '0');

					for (int i = 0; i < disp.length(); i++)
						tmp += disp[i];
					disp = tmp;
					if (disp.front() == '1')
					{
						mod = true;
						disp += " | " + string(8, '0');
					}
				}
				else
				{
					mod = true;
					string tmp = disp.substr(disp.length() - 8, 8);
					tmp += " | " + string(8 - disp.length()%8,'0') +  disp.substr(0, disp.length() % 8);
					disp = tmp;
				}
				
				
			}
			if (mod)
			{
				add += 2;;
				res += "10";
			}				
			else
			{
				add++;
				res += "01";
			}
				
			res += i2b(treg, 3);
			res += i2b(rm[operand.substr(1, 2)]);

			res+=" | " +  disp;
		}
		else // mod = 00
		{
			res += "00";
			res += i2b(treg, 3);
			res += i2b(rm[operand.substr(1,2)], 3);
		}
	}
	return res;
}

int parseDEC(vector<string>& words)
{
	int add = 1;
	string res;
	if (words[1] == "BYTE" || words[1] == "WORD")
	{	
		add++;
		char width = '0';
		if (words[1] == "WORD") width = '1';
		res += op[DEC_RM];
		res += width;

		string operand = words[3];
		bool with_disp = false;
		int i = 4;
		while (operand.back() != ']')
		{
			operand += words[i++];
			with_disp = true;
		}			

		res += " | " + modregrm(1, operand, with_disp, add);

		cout << res;
	}
	else
	{
		int index = reg[words[1]];
		if (index <= 7)
		{
			res += op[DEC_R];
			res += i2b(index, 3);
		}
		else
		{
			res += op[DEC_RM];
			res += '0';
			res += " | " + modregrm(1, words[1], false,add);
		}
		cout << res;
	}

	return add;
}

int parseXCHG(vector<string>& words)
{
	int add = 1;
	string res = "";
	words[1].erase(words[1].length()-1, 1);
	if (words.back().back() == ']') //xchg r, m
	{
		res = op[XCHG_R];
		if (words[1].back() == 'l' || words[1].back() == 'h')
			res += "0";
		else
			res += "1";
		string operand = words[2];
		bool with_disp = false;
		int i = 3;
		while (operand.back() != ']')
		{
			operand += words[i++];
			with_disp = true;
		}
		res += " | " + modregrm(reg[words[1]], operand, with_disp, add);
	}
	else if (reg[words[1]] == 0)  // xchg ax, r
	{
		res = op[XCHG_AX] + i2b(reg[words[2]],3);
	}
	else if (reg[words[2]] == 0)
	{
		res = op[XCHG_AX] + i2b(reg[words[1]], 3);
	}
	else // xchg r, r
	{
		res = op[XCHG_R];
		if (words[1].back() == 'l' || words[1].back() == 'h')
			res += "0";
		else
			res += "1";
		res += " | 11";
		
		res += i2b(reg[words[1]] % 8, 3);
		res += i2b(reg[words[2]] % 8, 3);
	}
	cout << res;
	return add;
}

int parse(string& line)
{
	stringstream str(line);
	vector<string> words;
	string tmp;
	while (getline(str, tmp, ' ')) {
		words.push_back(tmp);
	}
	if (words[0] == opDec)
		return parseDEC(words);
	else if (words[0] == opXchg)
		return parseXCHG(words);
	return 0;
	
}

int main()
{
	ifstream file("test.asm");
	if (file.is_open())
	{
		ofstream bin("test.obj", ios::binary);
		string line = " ";
		int i = 1;
		int address = 0;

		while (getline(file, line)) {
			printf_s("%2d %4s\t %35s", i++, i2h(address, 4).c_str(), line.c_str());
			cout << "\t\t";
			address += parse(line);
			cout << endl;
		}

		bin.close();
		file.close();
	}

	return 0;
}