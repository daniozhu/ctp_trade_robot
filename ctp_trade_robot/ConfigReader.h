#pragma once
#include <string>
#include <vector>

using StringVector = std::vector<std::wstring>;

class ConfigReader
{
public:
	ConfigReader(const std::wstring& sIniPath);
	~ConfigReader() = default;

	const std::wstring& GetMarketDataURL() const { return m_marketDataURL; }
	const std::wstring& GetMarketDataLocalDir() const { return m_marketDataLocalDir; }
	const StringVector& GetInstrumentIds() const { return m_instrumentIds; }

private:
	StringVector					m_instrumentIds;
	std::wstring					m_marketDataURL;
	std::wstring					m_marketDataLocalDir;
};

