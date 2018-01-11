#include "stdafx.h"

#include "ConfigReader.h"
#include "CtpLog.h"
#include "Util.h"

const static TCHAR* const APPNAME_MARKETDATA		= L"MarketData";
const static TCHAR* const KEYNAME_URL				= L"URL";
const static TCHAR* const KEYNAME_INSTRUMENTID		= L"InstrumentId";
const static TCHAR* const KEYNAME_LOCALDIR			= L"LocalDir";

const static TCHAR* const DEFAULT_STRING			= L"";

ConfigReader::ConfigReader(const std::wstring & sIniPath)
{
	if (!::PathFileExists(sIniPath.c_str()))
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"Configuration file does not exist - " + sIniPath);
		return;
	}
	
	TCHAR szValue[MAX_PATH] = { 0 };

	// Get market data url
	::GetPrivateProfileString(APPNAME_MARKETDATA, KEYNAME_URL, DEFAULT_STRING, szValue, MAX_PATH, sIniPath.c_str());
	m_marketDataURL = szValue;

	// Get local dir the market data being downloaded to
	::GetPrivateProfileString(APPNAME_MARKETDATA, KEYNAME_LOCALDIR, DEFAULT_STRING, szValue, MAX_PATH, sIniPath.c_str());
	m_marketDataLocalDir = szValue;

	// Get instrument ids that we want to get the market data
	::GetPrivateProfileString(APPNAME_MARKETDATA, KEYNAME_INSTRUMENTID, DEFAULT_STRING, szValue, MAX_PATH, sIniPath.c_str());

	Util::SplitCharToVector(szValue, m_instrumentIds);
}

