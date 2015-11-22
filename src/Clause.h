#include <unordered_map>
#include <iostream>
#include <vector>
#include "ultility.h"

using namespace std;

class Clause
{
public:
	vector<int*> literal_list;
	unordered_map<int*, int> literal2type;

	int val;
	int setted_literal_num;

	Clause(string s, vector<int*>& literal) {
		val = -1;

		vector<string> str_list = split(s, ' ');

		int index;
		for (int i = 0; i < str_list.size() - 1; i++)
		{
			index = fromString(str_list[i]);
			literal_list.push_back(literal[index]);
			literal2type[literal[index]] = (index < 0) ? 0 : 1;
		}
	}

	// return value res = last two bits of previous val + last two bits of current val
	int newLiteralSetted(int* xi) {

		++setted_literal_num;

		int res = (val & 0x3) << 2;

		if (!literal2type[xi] ^ (*xi))
			val = 1;
		else if (setted_literal_num == literal_list.size() && val == -1)
			val = 0;
		else
			val = -1;

		res |= (val & 0x3);

		return res;
	}

	// return value res = last two bits of previous val + last two bits of current val
	int update() {

		int res = (val & 0x3) << 2;
		setted_literal_num = 0;

		val = -1;
		for (int i = 0; i < literal_list.size(); i++)
		{
			if (!(*literal_list[i]) ^ literal2type[literal_list[i]])
				val = 1;

			setted_literal_num += (*literal_list[i] + 2) >> 1;
		}

		if (setted_literal_num == literal_list.size())
			val = 0;

		res |= (val & 0x3);

		return res;
			
	}
};