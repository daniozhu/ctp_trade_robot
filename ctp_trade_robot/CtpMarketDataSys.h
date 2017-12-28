#pragma once

class CThostFtdcMdApi;
class CtpMarketDataSpi;

class CtpMarketDataSys
{
public:
	CtpMarketDataSys();
	~CtpMarketDataSys();

	bool Start();

private:

	//CTP Market api/spi
	CThostFtdcMdApi*	m_pMdUserApi;
	CtpMarketDataSpi*	m_pMdUserSpi;


};

