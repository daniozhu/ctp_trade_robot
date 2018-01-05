#include "stdafx.h"
#include "CmdFile.h"
#include "CtpApp.h"

CmdFile * CmdFile::Get()
{
	static CmdFile  s_cmdFile;
	return &s_cmdFile;
}

void CmdFile::Refresh()
{
	wchar_t szValue[10] = { 0 };
	::GetPrivateProfileString(L"App", L"Terminate", L"", szValue, 10, m_cmdFilePath.c_str());
	m_bTerminatingApp = (_wcsicmp(szValue, L"true") == 0);
}

CmdFile::CmdFile()
	: m_bTerminatingApp(false)
{
	m_cmdFilePath = CtpApp::Get()->GetAppDir() + L"\\cmd.ini";
}

CmdFile::~CmdFile()
{
}
