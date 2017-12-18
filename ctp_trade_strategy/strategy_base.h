#pragma once
#include "strategy_system.h"

#include <vector>

#pragma warning (disable:4251)

class CTP_TRADE_STRATEGY_API Strategy
{
public:
	struct TradeSuggestion
	{
		enum class Level	{eStrong, eNormal, eLow};
		std::wstring		InstrumentId;
		bool				IsBuy;
		Level				Confidence;
	};

	using TradeSuggestions = std::vector<TradeSuggestion>;

	virtual ~Strategy() =  default;
	virtual bool Process() = 0;

	const TradeSuggestions& GetTradeSuggestions() const { return m_suggestions; }

protected:
	TradeSuggestions		m_suggestions;

};