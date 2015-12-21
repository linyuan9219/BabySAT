#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

class ultility
{
public:
	static vector<string> split(string s, char c) {
		vector<string> res;
		string tmp = "";

		for (int i = 0; i < s.size(); i++)
			if (s[i] == c)
			{
				res.push_back(tmp);
				tmp = "";
			}
			else
				tmp += s[i];

		if (tmp != "")
			res.push_back(tmp);

		return res;
	}

	static int fromString(string s)
	{
		stringstream ss;
		ss << s;
		int res;
		ss >> res;
		return res;
	}

	static string toString(int i)
	{
		stringstream ss;
		ss << i;
		string res;
		ss >> res;
		return res;
	}

	static vector<int> fromString(vector<string> s_list)
	{
		vector<int> res;
		for (int i = 0; i < s_list.size(); i++)
			res.push_back(fromString(s_list[i]));

		return res;
	}

	static int getBitNum(int s)
	{
		int res = 0;
		s += 1;
		while ((s = s / 2) > 0)
			++res;
		return res;
	}

	static string findLeftmostE(string s, int shift = 0)
	{
		int pos = shift;

		if (s[pos] != '(')
			for (int i = pos; i < s.size(); i++)
				if (s[i] == ' ')
					return s.substr(pos, i - pos);

		vector<int> stack;
		stack.push_back(0);

		for (int i = pos + 1; i < s.size(); i++)
		{
			if (s[i] == '(')
				stack.push_back(0);
			if (s[i] == ')')
				stack.pop_back();

			if (stack.empty())
				return s.substr(pos + 1, i - pos - 1);
		}
		return "";
	}

	static string findRightmostE(string s, int shift = 0)
	{
		int pos = s.size() - shift - 1;

		if (s[pos] != ')')
			for (int i = pos; i >= 0; i--)
				if (s[i] == ' ')
					return s.substr(i + 1, pos - i);

		vector<int> stack;
		stack.push_back(1);
		for (int i = pos - 1; i >= 0; i--)
		{
			if (s[i] == ')')
				stack.push_back(1);
			if (s[i] == '(')
				stack.pop_back();

			if (stack.empty())
				return s.substr(i + 1, pos - i - 1);
		}

		return "";
	}
};