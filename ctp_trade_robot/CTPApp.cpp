#include "stdafx.h"

#include "CTPApp.h"
#include "CtpLog.h"

CtpApp::CtpApp()
{
	TCHAR exeFullPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, exeFullPath, MAX_PATH);

	// Find last file path seperator
	TCHAR* pFind = _tcsrchr(exeFullPath, L'\\');
	if (nullptr == pFind) {
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"Invalid application path");
		return;
	}

	*pFind = L'\0';
	m_appDir = exeFullPath;
}

CtpApp * CtpApp::Get()
{
	static CtpApp s_ctpApp;
	return &s_ctpApp;
}

