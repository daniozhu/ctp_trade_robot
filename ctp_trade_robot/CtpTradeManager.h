#pragma once

#include "CtpPosition.h"

#include "../ctp_trade_strategy/CommonDataType.h"

#include <mutex>
#include <condition_variable>



class CThostFtdcMdApi;
class CtpMarketDataSpi;
class CThostFtdcTraderApi;
class CtpTraderSpi;

class CtpTradeRobot
{
	friend CtpTraderSpi;
public:
	CtpTradeRobot();
	~CtpTradeRobot();

	bool DownloadMarketData();
	bool ApplyStrategy();
	bool TradeInCTP();

private:
	StringMap			m_instrumentIdMarketData;

	//CTP Market api/spi
	CThostFtdcMdApi*	m_pMdUserApi;
	CtpMarketDataSpi*	m_pMdUserSpi;

	//CTP trader api/spi
	CThostFtdcTraderApi*	m_pTraderApi;
	CtpTraderSpi*			m_pTraderSpi;

	// Positions
	std::vector<CtpPosition>	m_positions;

	int					m_tradeRequestId;

	bool				m_bIsConnected;
	bool				m_bIsLogin;

	std::string			m_tradingDay;

	std::mutex					m_mutex;
	std::condition_variable		m_cond;
};


