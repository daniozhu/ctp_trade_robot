#include "stdafx.h"

#include "CtpLog.h"
#include "CTPApp.h"
#include "Util.h"

constexpr WORD  WHITE_COLOR  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
constexpr WORD  YELLOW_COLOR = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
constexpr WORD  RED_COLOR    = FOREGROUND_RED | FOREGROUND_INTENSITY;

CtpLog::CtpLog()
{
	m_sLogPath = CtpApp::Get()->GetAppDir();
	m_sLogPath.append(L"\\CtpLog.log");
}

std::wstring CtpLog::GetCurrentLocalTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[100];
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return Util::StringToWString(buf);
}

CtpLog * CtpLog::Get()
{
	static CtpLog s_Log;
	return &s_Log;
}

void CtpLog::Write(LogLevel leve, const std::wstring & rawMsg)
{
	std::wstring logMsg = GetCurrentLocalTime();
	Color textColor(WHITE_COLOR);

	switch (leve)
	{
	case LogLevel::eNormal:
		logMsg.append(L": ");
		break;
	case LogLevel::eWarning:
		textColor.m_color = YELLOW_COLOR;
		logMsg.append(L": [WARNING] ");
		break;
	case LogLevel::eError:
		textColor.m_color = RED_COLOR;
		logMsg.append(L": [ERROR] ");
		break;
	default:
		textColor.m_color = YELLOW_COLOR;
		logMsg.append(L": Please specify a proper log level, ");
	}

	// Append the raw message
	logMsg.append(rawMsg);

	// Output to console
	std::wcout << textColor << logMsg << std::endl;

	// Write to log file
	std::wofstream logFile;
	logFile.open(m_sLogPath, std::ios::app);
	logFile << logMsg << std::endl;
	logFile.close();
}
