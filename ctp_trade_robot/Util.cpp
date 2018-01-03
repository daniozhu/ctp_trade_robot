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

std::wstring Util::StringToWString(const std::string & srcString)
{
	const int nLen = MultiByteToWideChar(CP_ACP, 0, srcString.c_str(), -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszBuf = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, 0, srcString.c_str(), -1, pwszBuf, nLen);
	std::wstring wstr{ pwszBuf };
	delete[] pwszBuf;
	pwszBuf = nullptr;

	return std::move(wstr);
}

std::string Util::WStringToString(const std::wstring & srcWString)
{
	const int nLen = WideCharToMultiByte(CP_ACP, 0, srcWString.c_str(), -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
		return std::string("");

	char* pszDst = new char[nLen];
	if (!pszDst)
		return std::string("");

	WideCharToMultiByte(CP_ACP, 0, srcWString.c_str(), -1, pszDst, nLen, NULL, NULL);
	std::string str{ pszDst };
	delete[] pszDst;
	pszDst = nullptr;

	return std::move(str);
}

std::wstring Util::GetFileDir(const std::wstring & filePath)
{
	wchar_t szDriver[3];
	wchar_t	szDir[256];
	wchar_t szName[256];
	wchar_t szExt[256];

	_wsplitpath_s(filePath.c_str(), szDriver, szDir, szName, szExt);
	std::wstring dirPath = szDriver;
	dirPath.append(szDir);
	
	return std::move(dirPath);
}

FILETIME Util::GetFileLastModifiedTime(const std::wstring & filePath)
{
	FILETIME fileTime{ 0 };
	HANDLE hFile = ::CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return fileTime;

	::GetFileTime(hFile, NULL, NULL, &fileTime);
	::CloseHandle(hFile);

	return std::move(fileTime);
}
