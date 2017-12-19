#include "stdafx.h"
#include "CtpPosition.h"

#include "../tradeapi_x64/ThostFtdcUserApiStruct.h"

CtpPosition::CtpPosition(CThostFtdcInvestorPositionField *pField)
{
	if (pField != nullptr)
	{
		m_id = pField->InstrumentID;
		m_direction = pField->PosiDirection == '2' ? eDirection::eLong : eDirection::eShort;
		m_dateType = pField->PositionDate == '1' ? eDateType::eToday : eDateType::eHistory;
		m_volume = pField->Position;
		m_cost = pField->PositionCost;
		m_profit = pField->PositionProfit;
	}
}
