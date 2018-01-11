#pragma once
#include <string>

class CmdFile
{

public:
	static CmdFile* Get();

	void Refresh();
	bool IsTerminatingApp() const { return m_bTerminatingApp; }

	const std::vector<std::wstring>& GetOpenPositions() const { return m_openPositions; }

private:
	CmdFile();
	~CmdFile();

private:
	bool					m_bTerminatingApp;
	std::wstring		m_cmdFilePath;

	std::vector<std::wstring>	m_openPositions;
};

