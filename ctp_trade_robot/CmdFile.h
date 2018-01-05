#pragma once
#include <string>

class CmdFile
{

public:
	static CmdFile* Get();

	void Refresh();
	bool IsTerminatingApp() const { return m_bTerminatingApp; }

private:
	CmdFile();
	~CmdFile();

private:
	bool					m_bTerminatingApp;
	std::wstring		m_cmdFilePath;
};

