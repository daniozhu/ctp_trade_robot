#pragma once

class CThostFtdcMdApi;
class CtpMarketDataSpi;

class CtpMarketDataSys
{
	friend CtpMarketDataSpi;

public:
	bool Start();
	bool Stop();
	void Join();

	static CtpMarketDataSys* Get();

private:
	CtpMarketDataSys();
	~CtpMarketDataSys();

private:

	//CTP Market api/spi
	CThostFtdcMdApi*			m_pMdUserApi;
	CtpMarketDataSpi*			m_pMdUserSpi;
};

