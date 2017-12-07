#pragma once
#include "strategy_system.h"

#include <string>

class CTP_TRADE_STRATEGY_API ILog
{
public:
	enum class LogLevel { eNormal, eWarning, eError };

	virtual void Write(LogLevel leve, const std::wstring& rawMsg) = 0;

	ILog(const ILog&) = delete;
	ILog& operator=(const ILog&) = delete;

	virtual ~ILog() = default;

protected:
	ILog() = default;
};