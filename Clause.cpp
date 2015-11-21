#include <unordered_map>
#include <iostream>
#include <vector>
#include "ultility.h"

using namespace std;

class Clause
{
public:
	vector<int*> literal_list;
	unordered_map<int*, type> literal2type;

	int val;
	int setted_literal_num;

	Clause(string s, int* literal) {
		val = -1;

		vector<string> str_list = s.split(' ');

		int index;
		for (int i = 0; i < str_list.size(); i++)
		{
			index = fromString(str_list[i]);
			literal_list.push_back(literal + index);
			literal2type[literal + index] = (index < 0) ? 0 : 1;
		}
	}

	int newLiteralSetted(int* xi) {
		++setted_literal_num;
		if (!literal2tpye[xi] ^ xi)
			val = 1;
		else if (setted_literal_num == literal_list.size() && val == -1)
			val = 0;

		return val;
	}

	int update() {
		val = -1;
		setted_literal_num = 0;
		for (int i = 0; i < literal_list.size(); i++)
		{
			if (!literal_list[i] ^ literal2tpye[literal_list[i]])
			{
				val = 1;
				return val;
			}

			setted_literal_num += (literal_list[i] + 2) >> 1;
		}

		if (setted_literal_num == literal_list.size())
			val = 0;

		return val;
			
	}
};