#pragma once
#include "../tradeapi_x64/ThostFtdcMdApi.h"

#include "../ctp_trade_strategy/CommonDataType.h"

class CtpMarketDataSpi : public CThostFtdcMdSpi
{
	
public:
	CtpMarketDataSpi(CThostFtdcMdApi* pUserApi);
	virtual ~CtpMarketDataSpi();

	virtual void OnFrontConnected() override;
	virtual void OnFrontDisconnected(int nReason) override;
	
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;
	virtual void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;

	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) override;
	
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) override;

	// Set instruments that we want to receive the market data.
	void SetTargetInstruments(const NStringVector& instruments) { m_instruments = instruments; }

private:
	CThostFtdcMdApi*			m_pMdUserApi;
	NStringVector					m_instruments;
};

