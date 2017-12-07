#pragma once

#include "strategy_system.h"
#include "strategy_base.h"

#include <map>

class ILog;

using StringMap = std::map<std::wstring, std::wstring>;

class CTP_TRADE_STRATEGY_API Strategy_MA5_10 : public Strategy
{
public:
	Strategy_MA5_10(const StringMap& instrumentIdData, ILog* pLog = nullptr);
	virtual ~Strategy_MA5_10();

	virtual bool Process() override;

private:
	StringMap		m_instrumentIdData;
	ILog*			m_pLog;
};

