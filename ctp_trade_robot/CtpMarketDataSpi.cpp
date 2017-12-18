#include "stdafx.h"
#include "CtpMarketDataSpi.h"

#include "CtpLog.h"
#include "Util.h"

const char* g_BrokerId = "9999";
const char* g_UserId   = "082644";
const char* g_Password = "123456789";

static int g_RequestId = 0;

CtpMarketDataSpi::CtpMarketDataSpi(CThostFtdcMdApi* pUserApi)
	:m_pMdUserApi(pUserApi)
{
}

CtpMarketDataSpi::~CtpMarketDataSpi()
{
	m_pMdUserApi = nullptr;
}

void CtpMarketDataSpi::OnFrontConnected()
{
	CtpLog::Get()->Write(CtpLog::LogLevel::eNormal, L"MarketData OnFrontConnected");

	if (nullptr == m_pMdUserApi) 
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"MarketData User API is null");
		return;
	}

	// Log in
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy_s(req.BrokerID, g_BrokerId);
	strcpy_s(req.UserID, g_UserId);
	strcpy_s(req.Password, g_Password);

	int nRet = m_pMdUserApi->ReqUserLogin(&req, g_RequestId++);
	if (0 == nRet) {
		CtpLog::Get()->Write(CtpLog::LogLevel::eNormal, L"MarketData ReqUserLogin Succeeded");
	} else {
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"MarketData ReqUserLogin Failed");
	}
	
}

void CtpMarketDataSpi::OnFrontDisconnected(int nReason)
{
	CtpLog::Get()->Write(CtpLog::LogLevel::eNormal, L"MarketData OnFrontDisconnected");
}

void CtpMarketDataSpi::OnRspUserLogin(CThostFtdcRspUserLoginField * pRspUserLogin, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{ 
	const bool bOK = pRspInfo && pRspInfo->ErrorID == 0;
	if (bOK && pRspUserLogin != nullptr)
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eNormal, L"MarketData User Login Succeeded");

		// Subscrible instrument market data
		if (!m_instruments.empty())
		{
			char** ppInstruments = new char*[m_instruments.size()];
			for (size_t i =0 ; i < m_instruments.size(); ++i)
			{
				ppInstruments[i] = const_cast<char*>(m_instruments[i].c_str());
			}

			m_pMdUserApi->SubscribeMarketData(ppInstruments, (int)m_instruments.size());
			delete[] ppInstruments;
		}

		// Get existing position status
	}
	else
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"MarketData User Login Failed");
	}
}

void CtpMarketDataSpi::OnRspUserLogout(CThostFtdcUserLogoutField * pUserLogout, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	CtpLog::Get()->Write(CtpLog::LogLevel::eNormal, L"MarketData OnRspUserLogout");
}

void CtpMarketDataSpi::OnRspError(CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpMarketDataSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField * pSpecificInstrument, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo && pRspInfo->ErrorID == 0)
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eNormal, Util::StringToWString(pSpecificInstrument->InstrumentID));
	}
}

void CtpMarketDataSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField * pDepthMarketData)
{
	if (pDepthMarketData != nullptr)
	{
		std::cout << "InstrumentId: " << pDepthMarketData->InstrumentID
				  << ", UpdateTime: " << pDepthMarketData->UpdateTime
				  << ", LastPrice: " << pDepthMarketData->LastPrice
				  << ", Volume: " << pDepthMarketData->Volume << std::endl;
	}
}

void CtpMarketDataSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField * pSpecificInstrument, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpMarketDataSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField * pSpecificInstrument, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpMarketDataSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField * pSpecificInstrument, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpMarketDataSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField * pForQuoteRsp)
{
}
