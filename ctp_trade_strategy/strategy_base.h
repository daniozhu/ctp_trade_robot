#pragma once
#include "strategy_system.h"

#include <vector>

#pragma warning (disable:4251)

class CTP_TRADE_STRATEGY_API Strategy
{
public:
	struct TradeSuggestion
	{
		std::wstring	InstrumentId;
		bool			IsOpenPosition;
		bool			IsBuy;
		unsigned int	Hands;
	};

	using TradeSuggestions = std::vector<TradeSuggestion>;

	virtual ~Strategy() =  default;
	virtual bool Process() = 0;

	const TradeSuggestions& GetTradeSuggestions() const { return m_suggestions; }

protected:
	TradeSuggestions		m_suggestions;

};