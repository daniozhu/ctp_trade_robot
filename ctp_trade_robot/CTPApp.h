#pragma once
#include <string>

class CtpApp
{
public:
	static CtpApp* Get();

	const std::wstring& GetAppDir() const { return m_appDir; }

	bool IsTerminating() const			{ return m_bIsTerminating; }
	void SetTerminating()					{ m_bIsTerminating = true; }

private:
	CtpApp();
	~CtpApp() = default;

private:
	std::wstring			m_appDir;
	volatile bool		m_bIsTerminating;
};

