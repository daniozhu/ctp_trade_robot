#include "stdafx.h"
#include "Util.h"

void Util::FormatJsonData(const std::wstring & srcJsonFile, const std::wstring & destJsonFile)
{
	std::regex r("\\b([a-zA-Z]+):");

	std::ifstream fin(srcJsonFile.c_str(), std::ios::in);
	std::ofstream fout(destJsonFile.c_str(), std::ios::out);

	char str[MAX_PATH];

	while (fin.getline(str, MAX_PATH, '}'))
	{
		fout << regex_replace(str, r, "\"$1\":") << (fin.eof() ? "" : "}") << std::endl;
	}

	fout.close();
	fin.close();
}
