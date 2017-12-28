#include "stdafx.h"
#include "CtpMarketDataSys.h"

#include "CtpMarketDataSpi.h"
#include "../tradeapi_x64/ThostFtdcMdApi.h"

// Market Front
// standard: tcp://180.168.146.187:10010
// 7x24:     tcp://180.168.146.187:10031
char* marketfront_address = "tcp://180.168.146.187:10010";

CtpMarketDataSys::CtpMarketDataSys()
	:m_pMdUserApi(nullptr)
	,m_pMdUserSpi(nullptr)
{
}


CtpMarketDataSys::~CtpMarketDataSys()
{
	if (m_pMdUserApi) {
		m_pMdUserApi->RegisterSpi(nullptr);
		m_pMdUserApi->Release();
		m_pMdUserApi = nullptr;
	}

	delete m_pMdUserSpi;
	m_pMdUserSpi = nullptr;

}

bool CtpMarketDataSys::Start()
{
	// Market API
	m_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi("MarketDataFlow");
	m_pMdUserSpi = new CtpMarketDataSpi(m_pMdUserApi);

	m_pMdUserApi->RegisterSpi(m_pMdUserSpi);
	m_pMdUserApi->RegisterFront(marketfront_address);
	m_pMdUserApi->Init();

	m_pMdUserApi->Join();

	return true;
}
