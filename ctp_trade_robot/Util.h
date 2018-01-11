#pragma once
namespace Util
{
	void FormatJsonData(const std::wstring& srcJsonFile, const std::wstring& destJsonFile); 
	std::wstring StringToWString(const std::string& srcString);
	std::string  WStringToString(const std::wstring& srcWString);

	std::wstring GetFileDir(const std::wstring& filePath);
	FILETIME GetFileLastModifiedTime(const std::wstring& filePath);

	void SplitCharToVector(const wchar_t* pszValue, std::vector<std::wstring>& strVector);

};
