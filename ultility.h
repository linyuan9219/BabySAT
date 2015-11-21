#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<string> split(string s, char c) {
	vector<string> res;
	string tmp = "";

	for (int i = 0; i < s.szie(); i++)
		if (s[i] == c)
		{
			res.push_back(tmp);
			tmp = "";
		}
		else
			tmp += s[i];

	return res;
}

int fromString(string s)
{
	stringstream ss;
	ss << s;
	int res;
	ss >> res;
	return res;
}