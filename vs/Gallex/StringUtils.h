#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#pragma once

#include <Windows.h>
#include <string>
#include <tchar.h>
#include <vector>

using namespace std;

vector<string> GetXMLDataWithTag(const string& text, string tag);
vector<string> GetValuesFromXMLData(const string& text);
wstring StringToWString(const string& s);
string WCHARToString(wchar_t wc[260]);
double StringTimeToDouble(string& str_time);
double StringAngleToDouble(string& str_angle);
bool isNumber(const string& s);
bool compareStrings(std::string a, std::string b);
string GetStringTime(double system_time);
string RemoveZerro(string src);

#endif // !_STRING_UTILS_H_