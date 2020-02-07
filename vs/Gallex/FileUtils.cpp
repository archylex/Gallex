#include "FileUtils.h"

#define BUFFER_SIZE_F 256
string GetModuleDirectory()
{
	size_t i;
	char* buffer = new char[BUFFER_SIZE_F];
	wchar_t moduledirectory[BUFFER_SIZE_F];
	GetModuleFileName(GetModuleHandle(NULL), moduledirectory, BUFFER_SIZE_F);
	wcstombs_s(&i, buffer, (size_t)BUFFER_SIZE_F, moduledirectory, (size_t)BUFFER_SIZE_F);
	*(strrchr(buffer, '\\') + 1) = 0;
	string ModuleDirectory = buffer;
	delete[] buffer;
	return ModuleDirectory;
}

char* ReadFile(const string filename)
{
	string dirFilename = GetModuleDirectory() + filename;

	FILE* file;

	errno_t err = fopen_s(&file, dirFilename.c_str(), "rb");

	if (err != 0)
		return NULL;

	fseek(file, 0, SEEK_END);

	long size = ftell(file);

	fseek(file, 0, SEEK_SET);

	char* source = new char[size + 1];

	fread(source, 1, size, file);

	fclose(file);

	source[size] = 0;

	return source;
}

void ScanFolder(const string& path, const string& _type, vector<string>& v)
{
	WIN32_FIND_DATA data;
	HANDLE hFind;
	string temp = GetModuleDirectory() + path + "\\*";

	if ((hFind = FindFirstFile(StringToWString(temp).c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			string str_filename = WCHARToString(data.cFileName);
			size_t found = str_filename.find_last_of(".");
			string type = str_filename.substr(found + 1);

			if (type.compare(_type) == 0)
				v.push_back(str_filename);

		} while (FindNextFile(hFind, &data) != 0);

		FindClose(hFind);
	}
}

vector<STARINFO> GetValuesFromStarFile(const char* filename)
{
	vector<string> lines = GetXMLDataWithTag(ReadFile(filename), "star");
	vector<STARINFO> stars;

	for (string& line : lines)
	{
		vector<string> v = GetValuesFromXMLData(line);
		STARINFO info;
		info.name = v[0];
		info.identity = v[1];
		info.magnitude = (float)stof(v[2].c_str());
		info.spectral_class = v[3];
		info.right_ascension = StringTimeToDouble(v[4]);
		info.declination = StringAngleToDouble(v[5]);
		stars.push_back(info);
	}

	return stars;
}