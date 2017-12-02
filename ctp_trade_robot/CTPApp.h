#pragma once
#include <string>

class CtpApp
{
public:
	static CtpApp* Get();

	const std::wstring& GetAppDir() const { return m_appDir; }

private:
	CtpApp();
	~CtpApp() = default;

private:
	std::wstring		m_appDir;
};

