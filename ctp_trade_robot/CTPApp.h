#pragma once
#include <string>

class CTPApp
{
public:
	static CTPApp* Get();

	const std::wstring& GetAppDir() const { return m_appDir; }

private:
	CTPApp();
	~CTPApp() = default;

private:
	std::wstring		m_appDir;
};

