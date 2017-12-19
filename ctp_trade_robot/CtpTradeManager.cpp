#include "stdafx.h"
#include "CtpTradeManager.h"

#include "CMarketDataDownLoader.h"
#include "ConfigReader.h"
#include "CTPApp.h"
#include "Util.h"
#include "CtpLog.h"
#include "CtpMarketDataSpi.h"
#include "CtpTraderSpi.h"
#include "CtpPosition.h"


#include "../ctp_trade_strategy/strategy_ma5_10.h"

// CTP SDK
#include "../tradeapi_x64/ThostFtdcMdApi.h"
#include "../tradeapi_x64/ThostFtdcTraderApi.h"

#include <chrono>
#include <thread>

// Market Front
// standard: tcp://180.168.146.187:10010
// 7x24:     tcp://180.168.146.187:10031
char* marketfront_address = "tcp://180.168.146.187:10010";

// Trade Front
// 7x24:       tcp://180.168.146.187:10030
// standard:   tcp://180.168.146.187:10001
char* Trade_Front = "tcp://180.168.146.187:10001";

// If define _TEST_, we will add some instruments even there are no instruments match the strategy
#define _TEST_

//-1，表示网络连接失败；
//-2，表示未处理请求超过许可数；
//-3，表示每秒发送请求数超过许可数。
//

const int MAX_TRY = 20;
int nTry = 0;
int nResult = 0;

#define TRY_CALL(pCtpApi, funcName, pReqField, nReqId)			\
nTry = 0;														\
nResult = pCtpApi->##funcName(pReqField, nReqId);				\
while(nResult != 0 && nTry++ < MAX_TRY) {						\
	std::this_thread::sleep_for(std::chrono::milliseconds(500));\
	nResult = pCtpApi->##funcName(pReqField, nReqId);			\
}																\
if (nResult != 0) {												\
	std::cout << "Call the function failed, " << #funcName <<	\
	", return value = "	<< nResult << std::endl;				\
	return false;												\
}



////////////////////////////////////////
// CtpTradeRobot
////////////////////////////////////////

CtpTradeRobot::CtpTradeRobot()
	:m_pMdUserApi(nullptr)
	,m_pMdUserSpi(nullptr)
	,m_pTraderApi(nullptr)
	,m_pTraderSpi(nullptr)
	,m_tradeRequestId(0)
	,m_bIsLogin(false)
	,m_bIsConnected(false)
{

}

CtpTradeRobot::~CtpTradeRobot()
{
	m_pMdUserApi->RegisterSpi(nullptr);
	m_pMdUserApi->Release();
	m_pMdUserApi = nullptr;
	delete m_pMdUserSpi;
	m_pMdUserSpi = nullptr;

	m_pTraderApi->RegisterSpi(nullptr);
	m_pTraderApi->Release();
	m_pTraderApi = nullptr;
	delete m_pTraderSpi;
	m_pTraderSpi = nullptr;
}


bool CtpTradeRobot::DownloadMarketData()
{
	std::wstring sIniPath = CtpApp::Get()->GetAppDir() + L"\\config.ini";
	ConfigReader configReader{ sIniPath };

	const WStringVector& instrumentIds = configReader.GetInstrumentIds();
	std::wstring sURL = configReader.GetMarketDataURL();

	// download market data for each instrument id
	std::wstring sMarketDataLocalDir = CtpApp::Get()->GetAppDir() + L"\\RawMarketData";
	std::wstring sMarketDataLocalDirFormat = CtpApp::Get()->GetAppDir() + L"\\FormatMarketData";
	if (!::PathFileExists(sMarketDataLocalDir.c_str())) {
		::CreateDirectory(sMarketDataLocalDir.c_str(), nullptr);
	}
	if (!::PathFileExists(sMarketDataLocalDirFormat.c_str())) {
		::CreateDirectory(sMarketDataLocalDirFormat.c_str(), nullptr);
	}

	// TODO: delete old files under the folders

	// build download url for each instrument id
	bool bRet = false;
	for (const auto& id : instrumentIds) 
	{
		std::wstring sMarketDataLocalPath = sMarketDataLocalDir;
		sMarketDataLocalPath.append(L"\\");
		sMarketDataLocalPath.append(id);
		sMarketDataLocalPath.append(L".txt");

		bRet = CMarketDataDownLoader::Get()->Download(sURL + id, sMarketDataLocalPath);
		if (!bRet)
			continue;

		// Format the json files
		std::wstring sMarketDataFormatPath = sMarketDataLocalDirFormat;
		sMarketDataFormatPath.append(L"\\");
		sMarketDataFormatPath.append(id);
		sMarketDataFormatPath.append(L".txt");

		Util::FormatJsonData(sMarketDataLocalPath, sMarketDataFormatPath);

		// Add the instrument id and its formated market data to the map
		m_instrumentIdMarketData.emplace(id, sMarketDataFormatPath);
	}

	return true;
}

bool CtpTradeRobot::ApplyStrategy()
{
	// Pass the data map to strategy
	Strategy_MA5_10 strategy_M5_10{ m_instrumentIdMarketData, CtpLog::Get() };
	bool bRet = strategy_M5_10.Process();
	if (!bRet)
	{
		CtpLog::Get()->Write(ILog::LogLevel::eError, L"Strategy process failed!");
		return false;
	}

	const Strategy::TradeSuggestions& suggestions = strategy_M5_10.GetTradeSuggestions();
	if (suggestions.empty())
	{
		CtpLog::Get()->Write(ILog::LogLevel::eWarning, L"MA5/MA10 strategy doesn't give any trade suggestions");
		return false;
	}

	// Output trade suggestions to console
	for (const auto& suggestion : suggestions)
	{
		std::wstring confidence = L"";
		switch (suggestion.Confidence)
		{
		case Strategy::TradeSuggestion::Level::eStrong:
			confidence = L"Strong";
			break;
		case Strategy::TradeSuggestion::Level::eNormal:
			confidence = L"Normal";
			break;
		case Strategy::TradeSuggestion::Level::eLow:
			confidence = L"Low";
			break;
		default:
			// Normal
			std::wstring confidence = L"Normal";
		}

		std::wstring suggestionMsg = suggestion.InstrumentId;
		suggestionMsg.append(L": ");
		suggestionMsg.append((suggestion.IsBuy ? L"Buy" : L"Sale"));
		suggestionMsg.append(L"Confidence: ");
		suggestionMsg.append(confidence);

		// Write to log
		CtpLog::Get()->Write(CtpLog::LogLevel::eNormal, suggestionMsg);
	}

	return true;
}

bool CtpTradeRobot::TradeInCTP()
{
	if (m_pMdUserApi != nullptr || m_pMdUserSpi != nullptr)
	{
		CtpLog::Get()->Write(CtpLog::LogLevel::eError, L"CTP is already running in process");
		return false;
	}

	// Market API
	m_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi("MarketDataFlow");
	m_pMdUserSpi = new CtpMarketDataSpi(m_pMdUserApi);

	// Pass in instrument ids that we are interested.
	NStringVector instruments; 
	instruments.reserve(m_instrumentIdMarketData.size());
	std::for_each(m_instrumentIdMarketData.begin(), m_instrumentIdMarketData.end(),
		[&instruments](const auto& item) {instruments.push_back(Util::WStringToString(item.first)); });

#ifdef _TEST_
	if (instruments.empty())
		instruments.push_back("ni1803");
#endif

	m_pMdUserSpi->SetTargetInstruments(instruments);

	m_pMdUserApi->RegisterSpi(m_pMdUserSpi);
	m_pMdUserApi->RegisterFront(marketfront_address);

	// Trader API
	m_pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi("TraderDataFlow");
	m_pTraderSpi = new CtpTraderSpi(m_pTraderApi, this);
	
	m_pTraderApi->RegisterSpi(m_pTraderSpi);
	m_pTraderApi->RegisterFront(Trade_Front);
	m_pTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);
	m_pTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);

	// Start the market and trader threads.
	//m_pMdUserApi->Init();
	m_pTraderApi->Init();

	// Wait for connected
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		m_cond.wait(lk, [this]() {return this->m_bIsConnected; });
	}
		
	CThostFtdcReqUserLoginField loginField{ 0 };
	strcpy_s(loginField.BrokerID, g_BrokerId);
	strcpy_s(loginField.UserID, g_UserId);
	strcpy_s(loginField.Password, g_Password);
	TRY_CALL(m_pTraderApi, ReqUserLogin, &loginField, m_tradeRequestId++);
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		m_cond.wait(lk, [this]() {return this->m_bIsLogin; });
	}

	// Must do ReqQryNotice and ReqSettlementInfoConfirm in each trading day
	// 1: Query notice
	CThostFtdcQryNoticeField noticeField{ 0 };
	strcpy_s(noticeField.BrokerID, g_BrokerId);
	TRY_CALL(m_pTraderApi, ReqQryNotice, &noticeField, m_tradeRequestId++);

	// 2: Confirm settlement
	CThostFtdcSettlementInfoConfirmField settleInfoConfirmReq{ 0 };
	strcpy_s(settleInfoConfirmReq.BrokerID, g_BrokerId);
	strcpy_s(settleInfoConfirmReq.InvestorID, g_UserId);
	TRY_CALL(m_pTraderApi, ReqSettlementInfoConfirm, &settleInfoConfirmReq, m_tradeRequestId++);

	// Query investor
	CThostFtdcQryInvestorField reqInvestor{ 0 };
	strcpy_s(reqInvestor.BrokerID, g_BrokerId);
	strcpy_s(reqInvestor.InvestorID, g_UserId);
	TRY_CALL(m_pTraderApi, ReqQryInvestor, &reqInvestor, m_tradeRequestId++);

	UpdatePositions();

	//m_pMdUserApi->Join();
	m_pTraderApi->Join();

	return true;
}

// Get current positions
bool CtpTradeRobot::UpdatePositions()
{
	m_positions.clear();

	CThostFtdcQryInvestorPositionField req{ 0 };
	strcpy_s(req.BrokerID, g_BrokerId);
	strcpy_s(req.InvestorID, g_UserId);
	TRY_CALL(m_pTraderApi, ReqQryInvestorPosition, &req, m_tradeRequestId++);
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		m_cond.wait(lk);
	}

	return true;
}


