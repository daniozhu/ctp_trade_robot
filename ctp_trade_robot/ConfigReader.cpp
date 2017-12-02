#include "stdafx.h"
#include "ConfigReader.h"

const static TCHAR* const APPNAME_MARKETDATA		= L"MarketData";
const static TCHAR* const KEYNAME_URL				= L"URL";
const static TCHAR* const KEYNAME_INSTRUMENTID		= L"InstrumentId";
const static TCHAR* const KEYNAME_LOCALDIR			= L"LocalDir";

const static TCHAR* const DEFAULT_STRING			= L"";

ConfigReader::ConfigReader(const std::wstring & sIniPath)
{
	assert(::PathFileExists(sIniPath.c_str())==TRUE && L"Configuration file doesn't exist");

	if (::PathFileExists(sIniPath.c_str()))
	{
		TCHAR szValue[MAX_PATH] = { 0 };

		// Get market data url
		::GetPrivateProfileString(APPNAME_MARKETDATA, KEYNAME_URL, DEFAULT_STRING, szValue, MAX_PATH, sIniPath.c_str());
		m_marketDataURL = szValue;

		// Get local dir the market data being downloaded to
		::GetPrivateProfileString(APPNAME_MARKETDATA, KEYNAME_LOCALDIR, DEFAULT_STRING, szValue, MAX_PATH, sIniPath.c_str());
		m_marketDataLocalDir = szValue;

		// Get instrument ids that we want to get the market data
		::GetPrivateProfileString(APPNAME_MARKETDATA, KEYNAME_INSTRUMENTID, DEFAULT_STRING, szValue, MAX_PATH, sIniPath.c_str());
		
		TCHAR temp[MAX_PATH] = { 0 };

		int index = 0;
		TCHAR* p = szValue;
		for (; *p != L'\0'; ++p)
		{
			if (*p != L' ' && *p != ';')
			{
				temp[index] = *p;
				index++;
			}
			else if (*p == L';')
			{
				m_instrumentIds.push_back(temp);

				memset(temp, L'\0', sizeof(temp));
				index = 0;
			}
		}

		// last element
		if(wcsnlen_s(temp, MAX_PATH) > 0)
			m_instrumentIds.push_back(temp);
	}
}

