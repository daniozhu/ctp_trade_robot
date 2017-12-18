#pragma once
namespace Util
{
	void FormatJsonData(const std::wstring& srcJsonFile, const std::wstring& destJsonFile); 
	std::wstring StringToWString(const std::string& srcString);
	std::string  WStringToString(const std::wstring& srcWString);

};
