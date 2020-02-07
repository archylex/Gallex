#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "StringUtils.h"

using namespace std;

struct STARINFO {
	string name;
	string identity;
	double magnitude;
	string spectral_class;
	double right_ascension;
	double declination;
};

string GetModuleDirectory();
char* ReadFile(const string filename);
void ScanFolder(const string& path, const string& _type, vector<string>& v);
vector<STARINFO> GetValuesFromStarFile(const char* filename);

#endif // !_FILE_UTILS_H_