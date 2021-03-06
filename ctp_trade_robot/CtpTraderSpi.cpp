#include "stdafx.h"
#include "CtpTraderSpi.h"
#include "CtpTradeSys.h"


#include "../tradeapi_x64/ThostFtdcTraderApi.h"

static int g_tradeReqId = 0;

CtpTraderSpi::CtpTraderSpi(CThostFtdcTraderApi* pTraderUserApi, CtpTradeSys* pTradeRobot)
	: m_pTraderUserApi(pTraderUserApi)
	, m_pTradeRobot(pTradeRobot)
{
}

CtpTraderSpi::~CtpTraderSpi()
{
	m_pTraderUserApi = nullptr;
	m_pTradeRobot = nullptr;
}

void CtpTraderSpi::OnFrontConnected()
{
	std::cout << "CtpTraderSpi::OnFrontConnected" << std::endl;

	m_pTradeRobot->m_bIsConnected = true;
	
	m_pTradeRobot->m_cond.notify_all();
}

void CtpTraderSpi::OnFrontDisconnected(int nReason)
{
	std::cout << "OnFrontDisconnected due to reason: " << nReason << std::endl;

	// Lock?
	m_pTradeRobot->m_bIsConnected = false;
}

void CtpTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField * pRspAuthenticateField, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField * pRspUserLogin, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspUserLogin" << std::endl;

	const bool bOK = pRspInfo && pRspInfo->ErrorID == 0;
	if (!bOK) {
		std::cout << "OnRespUserLogin failed - " << pRspInfo->ErrorMsg << std::endl;
	}

//	std::unique_lock<std::mutex> lk(m_pTradeRobot->m_mutex);
	m_pTradeRobot->m_bIsLogin = bOK;
	m_pTradeRobot->m_tradingDay = pRspUserLogin->TradingDay;

	m_pTradeRobot->m_cond.notify_all();
}

void CtpTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField * pInvestorPosition, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	if(pInvestorPosition != nullptr)
		m_pTradeRobot->m_positions.emplace_back(pInvestorPosition);

	if (bIsLast)
		m_pTradeRobot->m_cond.notify_all();
}

void CtpTraderSpi::OnRspQryInvestor(CThostFtdcInvestorField * pInvestor, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInvestor != nullptr) {
		std::cout << "InvestorId: " << pInvestor->InvestorID << ", Investor Name: " << pInvestor->InvestorName << std::endl;
	}

	if (bIsLast)
		m_pTradeRobot->m_cond.notify_all();
}


void CtpTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField * pUserLogout, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "CtpTraderSpi::OnRspUserLogout" << std::endl;
	m_pTradeRobot->m_bIsLogin = false;
}

void CtpTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField * pInputOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
	if (pRspInfo != nullptr && pRspInfo->ErrorID != 0) {
			std::cout << pRspInfo->ErrorMsg << std::endl;
	}
}

void CtpTraderSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField * pParkedOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField * pParkedOrderAction, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField * pInputOrderAction, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
}

void CtpTraderSpi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField * pQueryMaxOrderVolume, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField * pSettlementInfoConfirm,
	CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << "OnRspSettlementInfoConfirm" << std::endl;
	if (pSettlementInfoConfirm != nullptr)
	{
		std::cout << "Confirm Date: " << pSettlementInfoConfirm->ConfirmDate
			<< ", Confirm Time: " << pSettlementInfoConfirm->ConfirmTime << std::endl;
	}
}

void CtpTraderSpi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField * pRemoveParkedOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField * pRemoveParkedOrderAction, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspExecOrderInsert(CThostFtdcInputExecOrderField * pInputExecOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
}

void CtpTraderSpi::OnRspExecOrderAction(CThostFtdcInputExecOrderActionField * pInputExecOrderAction, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspForQuoteInsert(CThostFtdcInputForQuoteField * pInputForQuote, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQuoteInsert(CThostFtdcInputQuoteField * pInputQuote, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQuoteAction(CThostFtdcInputQuoteActionField * pInputQuoteAction, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryOrder(CThostFtdcOrderField * pOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryTrade(CThostFtdcTradeField * pTrade, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField * pInstrumentMarginRate, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField * pInstrumentCommissionRate, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField * pDepthMarketData, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField * pSettlementInfo, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField * pInvestorPositionDetail, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryNotice(CThostFtdcNoticeField * pNotice, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	if (pNotice != nullptr) {
		std::cout << pNotice->Content << std::endl;
	}
}

void CtpTraderSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField * pSettlementInfoConfirm, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField * pInvestorPositionCombineDetail, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField * pInstrumentOrderCommRate, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField * pOptionInstrTradeCost, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField * pOptionInstrCommRate, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryExecOrder(CThostFtdcExecOrderField * pExecOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryForQuote(CThostFtdcForQuoteField * pForQuote, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryQuote(CThostFtdcQuoteField * pQuote, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspError(CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
	std::cout << __FUNCTION__ << std::endl;
}

void CtpTraderSpi::OnRtnOrder(CThostFtdcOrderField * pOrder)
{
	std::cout << __FUNCTION__ << std::endl;

	if (pOrder != nullptr)
	{
		std::string status = "";
		switch (pOrder->OrderStatus)
		{
		case '0':
			status = "全部成交";
			break;
		case '1':
			status = "部分成交还在队列中";
			break;
		case '2':
			status = "部分成交不在队列中";
			break;
		case '3':
			status = "未成交还在队列中";
			break;
		case '4':
			status = "未成交不在队列中";
			break;
		case '5':
			status = "撤单";
			break;
		case 'a':
			status = "未知";
			break;
		case 'b':
			status = "尚未触发";
			break;
		case 'c':
			status = "已触发";
			break;

		default:
			status = "未知";
		}
		std::cout << status << std::endl;

		std::string submitStatus = "";
		switch (pOrder->OrderSubmitStatus)
		{
		case '0':
			submitStatus = "已经提交";
			break;
		case '1':
			submitStatus = "撤单已经提交";
			break;
		case '2':
			submitStatus = "修改已经提交";
			break;
		case '3':
			submitStatus = "已经接受";
			break;
		case '4':
			submitStatus = "报单已经被拒绝";
			break;
		case '5':
			submitStatus = "撤单已经被拒绝";
			break;
		case '6':
			submitStatus = "改单已经被拒绝";
			break;
		default:
			submitStatus = "Unknow";
		}
		std::cout << submitStatus << std::endl;
	}
}

void CtpTraderSpi::OnRtnTrade(CThostFtdcTradeField * pTrade)
{
	std::cout << __FUNCTION__ << std::endl;

	if (pTrade != nullptr)
	{
		// TODO
	}
}

void CtpTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField * pInputOrder, CThostFtdcRspInfoField * pRspInfo)
{
}

void CtpTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField * pOrderAction, CThostFtdcRspInfoField * pRspInfo)
{
}

void CtpTraderSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField * pInstrumentStatus)
{
}

void CtpTraderSpi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField * pTradingNoticeInfo)
{
}

void CtpTraderSpi::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField * pErrorConditionalOrder)
{
}

void CtpTraderSpi::OnRtnExecOrder(CThostFtdcExecOrderField * pExecOrder)
{
}

void CtpTraderSpi::OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField * pInputExecOrder, CThostFtdcRspInfoField * pRspInfo)
{
}

void CtpTraderSpi::OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField * pExecOrderAction, CThostFtdcRspInfoField * pRspInfo)
{
}

void CtpTraderSpi::OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField * pInputForQuote, CThostFtdcRspInfoField * pRspInfo)
{
}

void CtpTraderSpi::OnRtnQuote(CThostFtdcQuoteField * pQuote)
{
}

void CtpTraderSpi::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField * pInputQuote, CThostFtdcRspInfoField * pRspInfo)
{
}

void CtpTraderSpi::OnErrRtnQuoteAction(CThostFtdcQuoteActionField * pQuoteAction, CThostFtdcRspInfoField * pRspInfo)
{
}

void CtpTraderSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField * pForQuoteRsp)
{
}

void CtpTraderSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField * pParkedOrder, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField * pParkedOrderAction, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField * pTradingNotice, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}

void CtpTraderSpi::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField * pBrokerTradingAlgos, CThostFtdcRspInfoField * pRspInfo, int nRequestID, bool bIsLast)
{
}


