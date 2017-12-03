#pragma once

#include "strategy_system.h"
#include "strategy_base.h"

class CTP_TRADE_STRATEGY_API Strategy_MA5_10 : public Strategy
{
public:
	Strategy_MA5_10(void);

	virtual bool Apply() override;
};

