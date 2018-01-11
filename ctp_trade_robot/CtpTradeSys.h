#pragma once

#include "CtpPosition.h"

#include "../ctp_trade_strategy/CommonDataType.h"

#include <mutex>
#include <condition_variable>


class CThostFtdcTraderApi;
class CtpTraderSpi;
struct CThostFtdcInputOrderField;

class CtpTradeSys
{
	friend CtpTraderSpi;

public:
	//bool DownloadMarketData();
	//bool ApplyStrategy();
	bool Start();

	//bool UpdatePositions();

	bool Stop();
	void Join();

	static CtpTradeSys* Get();

	// Trade actions
	bool InsertOrder(CThostFtdcInputOrderField *pInputOrder);

private:
	CtpTradeSys();
	~CtpTradeSys();

private:
	StringMap									m_instrumentIdMarketData;

	//CTP trader api/spi
	CThostFtdcTraderApi*					m_pTraderApi;
	CtpTraderSpi*								m_pTraderSpi;

	// Positions
	std::vector<CtpPosition>				m_positions;

	int												m_tradeRequestId;

	bool												m_bIsConnected;
	bool												m_bIsLogin;

	std::string									m_tradingDay;

	std::mutex									m_mutex;
	std::condition_variable				m_cond;
};


