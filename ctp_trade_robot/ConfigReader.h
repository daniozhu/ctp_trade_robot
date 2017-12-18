#pragma once

#include "../ctp_trade_strategy/CommonDataType.h"

#include <string>
#include <vector>

class ConfigReader
{
public:
	ConfigReader(const std::wstring& sIniPath);
	~ConfigReader() = default;

	const std::wstring& GetMarketDataURL() const { return m_marketDataURL; }
	const std::wstring& GetMarketDataLocalDir() const { return m_marketDataLocalDir; }
	const WStringVector& GetInstrumentIds() const { return m_instrumentIds; }

private:
	WStringVector					m_instrumentIds;
	std::wstring					m_marketDataURL;
	std::wstring					m_marketDataLocalDir;
};

