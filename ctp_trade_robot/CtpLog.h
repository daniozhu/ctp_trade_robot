#pragma once
#include <iostream>
#include <windows.h>

class CtpLog
{
public:
	enum class LogLevel {eNormal, eWarning, eError};

	static CtpLog* Get();
	void Write(LogLevel leve, const std::wstring& rawMsg);

private:
	CtpLog();
	~CtpLog() = default;

	std::wstring GetCurrentLocalTime();

private:
	std::wstring	m_sLogPath;
};

inline std::ostream& white(std::ostream& o)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	return o;
}

struct Color
{
	Color(WORD attr) : m_color(attr) {}
	WORD m_color;
};

template<class _Elem, class _Traits>
std::basic_ostream<_Elem, _Traits>& operator <<(std::basic_ostream<_Elem, _Traits>& o, Color& c)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, c.m_color);

	return o;
}




