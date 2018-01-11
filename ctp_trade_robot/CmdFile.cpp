#include "stdafx.h"
#include "CmdFile.h"
#include "CtpApp.h"
#include "Util.h"

CmdFile * CmdFile::Get()
{
	static CmdFile  s_cmdFile;
	return &s_cmdFile;
}

void CmdFile::Refresh()
{
	wchar_t szValue[MAX_PATH] = { 0 };
	::GetPrivateProfileString(L"App", L"Terminate", L"", szValue, MAX_PATH, m_cmdFilePath.c_str());
	m_bTerminatingApp = (_wcsicmp(szValue, L"true") == 0);

	::memset(szValue, 0, sizeof(szValue));
	::GetPrivateProfileString(L"Trade", L"OpenPosition", L"", szValue, MAX_PATH, m_cmdFilePath.c_str());

	m_openPositions.clear();
	Util::SplitCharToVector(szValue, m_openPositions);
}

CmdFile::CmdFile()
	: m_bTerminatingApp(false)
{
	m_cmdFilePath = CtpApp::Get()->GetAppDir() + L"\\cmd.ini";
}

CmdFile::~CmdFile()
{
}
