#include "StringUtils.h"

vector<string> GetXMLDataWithTag(const string& text, string tag)
{
	vector<string> result;
	unsigned int pos = 0;
	unsigned start = 0;

	while (true)
	{
		start = text.find("<" + tag, pos);   if (start == string::npos) return result;

		start++;

		pos = text.find(" />", start);   if (pos == string::npos) return result;
		result.push_back(text.substr(start, pos - start));
	}
}

vector<string> GetValuesFromXMLData(const string& text)
{
	vector<string> result;
	unsigned int pos = 0;
	unsigned int start = 0;

	while (true)
	{
		start = text.find("\"", start); if (start == string::npos) return result;

		start++;

		pos = text.find("\"", start);   if (pos == string::npos) return result;

		result.push_back(text.substr(start, pos - start));

		start = pos + 1;
	}
}

wstring StringToWString(const string& s)
{
	int string_length = (int)s.length() + 1;
	int length = MultiByteToWideChar(CP_ACP, 0, s.c_str(), string_length, 0, 0);
	wchar_t* buffer = new wchar_t[length];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), string_length, buffer, length);
	wstring result(buffer);
	delete[] buffer;
	return result;
}

string WCHARToString(wchar_t wc[260])
{
	char ch[260];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, wc, -1, ch, 260, &DefChar, NULL);

	std::string res(ch);

	return res;
}

double StringTimeToDouble(string& str_time)
{
	int pos = str_time.find("h", 0);
	double time = (double)stod(str_time.substr(0, pos).c_str());
	int new_pos = str_time.find("m", pos);
	time += (double)stod(str_time.substr(pos + 2, new_pos).c_str()) / 60.0f;
	return time;
}

double StringAngleToDouble(string& str_angle)
{
	int pos = str_angle.find("\u00B0", 0);
	double angle = (double)stod(str_angle.substr(0, pos).c_str());
	int new_pos = str_angle.find("'", pos);
	double min = (double)stod(str_angle.substr(pos + 2, new_pos).c_str()) / 60.0f;
	angle = (angle >= 0) ? angle + min : angle - min;
	return angle;
}

bool isNumber(const std::string& s)
{
	return !s.empty() && s.find_first_not_of("0123456789.") == std::string::npos;
}

bool compareStrings(std::string a, std::string b)
{
	return a < b;
}

string GetStringTime(double system_time)
{
	int d = (int)system_time / 86400;
	int h = (int)system_time % 86400 / 3600;
	int m = (int)system_time % 86400 % 3600 / 60;
	int s = (int)system_time % 86400 % 3600 % 60;

	return to_string(d) + "d " + to_string(h) + "h " + to_string(m) + "m " + to_string(s) + "s";
}

string RemoveZerro(string src)
{
	src.erase(src.find_last_not_of('0') + 1, std::string::npos);
	if (src[src.size() - 1] == '.')
		src.erase(src.size() - 1);
	return src;
}