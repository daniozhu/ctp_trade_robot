// ctptraderobot.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "CMarketDataDownLoader.h"
#include "ConfigReader.h"
#include "CTPApp.h"
#include "Util.h"

using StringMap = std::map<std::wstring, std::wstring>;

int main()
{
	std::wstring sIniPath = CTPApp::Get()->GetAppDir() + L"\\config.ini";
	ConfigReader configReader{sIniPath};

	const StringVector& instrumentIds = configReader.GetInstrumentIds();
	std::wstring sURL = configReader.GetMarketDataURL();

	// download market data for each instrument id
	std::wstring sMarketDataLocalDir		= CTPApp::Get()->GetAppDir() + L"\\RawMarketData";
	std::wstring sMarketDataLocalDirFormat	= CTPApp::Get()->GetAppDir() + L"\\FormatMarketData";
	if (!::PathFileExists(sMarketDataLocalDir.c_str())) {
		::CreateDirectory(sMarketDataLocalDir.c_str(), nullptr);
	}
	if (!::PathFileExists(sMarketDataLocalDirFormat.c_str())) {
		::CreateDirectory(sMarketDataLocalDirFormat.c_str(), nullptr);
	}

	// TODO: delete old files under the folders

	// build download url for each instrument id
	StringMap instrumentIdMarketData;
	bool bRet = false;
	for (const auto& id : instrumentIds) {
		std::wstring sMarketDataLocalPath = sMarketDataLocalDir;
		sMarketDataLocalPath.append(L"\\");
		sMarketDataLocalPath.append(id);
		sMarketDataLocalPath.append(L".txt");

		bRet = CMarketDataDownLoader::Get()->Download(sURL + id, sMarketDataLocalPath);
		if (!bRet)
			continue;

		// Format the json files
		std::wstring sMarketDataFormatPath = sMarketDataLocalDirFormat;
		sMarketDataFormatPath.append(L"\\");
		sMarketDataFormatPath.append(id);
		sMarketDataFormatPath.append(L".txt");

		Util::FormatJsonData(sMarketDataLocalPath, sMarketDataFormatPath);
		
		// Add the instrument id and its formated market data to the map
		instrumentIdMarketData.emplace(id, sMarketDataFormatPath);
	}
	
    return 0;
}

