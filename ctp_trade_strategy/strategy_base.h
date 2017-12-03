#pragma once
#include "strategy_system.h"

class CTP_TRADE_STRATEGY_API Strategy
{
public:
	virtual bool Apply() = 0;
};