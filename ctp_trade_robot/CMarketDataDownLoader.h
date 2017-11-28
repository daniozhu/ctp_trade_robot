#pragma once
#include <string>


class CMarketDataDownLoader
{
private:
	CMarketDataDownLoader() = default;
	~CMarketDataDownLoader() = default;

public:
	bool Download(const std::wstring& sURL, const std::wstring& sLocalPath);
	static CMarketDataDownLoader* Get();
};

