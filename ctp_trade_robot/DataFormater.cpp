#include "stdafx.h"

#include "DataFormater.h"

////////////////////////////////////////////////////
// Json Data formater implementation
////////////////////////////////////////////////////
JsonDataFormater::JsonDataFormater(const std::wstring & srcFilePath, const std::wstring & formatedFilePath)
	: m_srcFilePath(srcFilePath)
	, m_formatedPath(formatedFilePath)
{
}

bool JsonDataFormater::Format()
{
	std::regex r("\\b([a-zA-Z]+):");

	std::ifstream fin(m_srcFilePath.c_str(), std::ios::in);
	std::ofstream fout(m_formatedPath.c_str(), std::ios::out);

	char str[MAX_PATH];

	while (fin.getline(str, MAX_PATH, '}'))
	{
		fout << regex_replace(str, r, "\"$1\":") << (fin.eof() ? "" : "}") << std::endl;
	}

	fout.close();
	fin.close();

	return true;
}

