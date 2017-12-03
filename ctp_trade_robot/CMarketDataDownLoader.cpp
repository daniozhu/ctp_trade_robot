#include "stdafx.h"

#include "CMarketDataDownLoader.h"
#include "CtpLog.h"

const static unsigned int MAX_BLOCK_SIZE = 1024;

#pragma comment(lib, "wininet.lib")

bool CMarketDataDownLoader::Download(const std::wstring & sURL, const std::wstring& sLocalPath)
{
	HINTERNET hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (NULL == hInternet) {
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"Open internet failed - ");
		return false;
	}

	HINTERNET hRequest = InternetOpenUrl(hInternet, sURL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	if (NULL == hRequest)
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"Open URL failed - " + sURL);
		InternetCloseHandle(hInternet);
		return false;
	}

	// Check status
	DWORD dwStatusCode;
	DWORD dwStatusSize = sizeof(dwStatusCode);
	HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwStatusSize, NULL);
	if (dwStatusCode != 200)
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"Download status is failed - " + sURL);
		InternetCloseHandle(hInternet);
		InternetCloseHandle(hRequest);
		return false;
	}

	DWORD dwSize = 0;
	DWORD dwSizeLength = sizeof(dwSize);
	HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwSize, &dwSizeLength, NULL);
	if (dwSize < 0)
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"Download file size less than zero - " + sURL);
		InternetCloseHandle(hInternet);
		InternetCloseHandle(hRequest);
		return false;
	}

	HANDLE hFile = CreateFile(sLocalPath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
									0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"Create local market data file failed - " + sLocalPath);
		InternetCloseHandle(hInternet);
		InternetCloseHandle(hRequest);
		return false;
	}

	// Read data
	byte buff[MAX_BLOCK_SIZE];
	DWORD byteRead = 0;

	memset(buff, 0, sizeof(buff));

	BOOL bRet = FALSE;
	DWORD byteWritten = 0;

	while (true)
	{
		bRet = InternetReadFile(hRequest, buff, MAX_BLOCK_SIZE - 1, &byteRead);
		if (bRet == FALSE || byteRead ==0) 
			break;

		bRet = WriteFile(hFile, buff, byteRead, &byteWritten, NULL);
		if (bRet == FALSE)
			break;
	}
	
	InternetCloseHandle(hInternet);
	InternetCloseHandle(hRequest);
	CloseHandle(hFile);

	if (bRet)
		CtpLog::Get()->Write(CtpLog::LogLevel::eNormal, L"Download and save market data successfully - " + sURL);
	else
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"Download and save market data failed - " + sURL);

	return bRet == TRUE ? true : false;
}

CMarketDataDownLoader * CMarketDataDownLoader::Get()
{
	static CMarketDataDownLoader s_downLoader;
	return &s_downLoader;
}
