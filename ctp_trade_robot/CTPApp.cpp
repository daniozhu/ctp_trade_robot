#include "stdafx.h"
#include "CTPApp.h"


CTPApp::CTPApp()
{
	TCHAR exeFullPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, exeFullPath, MAX_PATH);

	// Find last file path seperator
	TCHAR* pFind = _tcsrchr(exeFullPath, L'\\');
	assert(pFind != nullptr && L"Invalid application path");
	if (pFind != nullptr)
	{
		*pFind = L'\0';
		m_appDir = exeFullPath;
	}
}

CTPApp * CTPApp::Get()
{
	static CTPApp s_ctpApp;
	return &s_ctpApp;
}

