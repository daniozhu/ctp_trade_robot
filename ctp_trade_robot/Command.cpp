#include "stdafx.h"
#include "Command.h"
#include "CTPApp.h"
#include "CtpMarketDataSys.h"
#include "CtpTradeSys.h"
#include "FileMonitor.h"
#include "Util.h"

#include "../tradeapi_x64/ThostFtdcUserApiStruct.h"


extern const char* g_BrokerId;
extern const char* g_UserId;
extern const char* g_Password;

Command::Command()
{
}


Command::~Command()
{
}

void TerminateCommand::Execute()
{
	FileMonitor::Get()->Kill();
	FileMonitor::Get()->Join();

	CtpTradeSys::Get()->Stop();
	CtpTradeSys::Get()->Join();

	CtpMarketDataSys::Get()->Stop();
	CtpMarketDataSys::Get()->Join();

	CtpApp::Get()->SetTerminating();
}

OpenPositionCommand::OpenPositionCommand(const std::vector<std::wstring>& instrumentIds)
	:m_instrumentIds(instrumentIds)
{
}

void OpenPositionCommand::Execute()
{
	if (m_instrumentIds.empty())
		return;

	CThostFtdcInputOrderField reqField = { 0 };
	strcpy_s(reqField.BrokerID, g_BrokerId);
	strcpy_s(reqField.InvestorID, g_UserId);
	strcpy_s(reqField.UserID, g_UserId);

	reqField.Direction = THOST_FTDC_D_Buy;
	reqField.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	reqField.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	reqField.VolumeTotalOriginal = 1;

	reqField.VolumeCondition = THOST_FTDC_VC_AV; // Any Volume
	reqField.MinVolume = 1;
	reqField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	reqField.IsAutoSuspend = 0;
	reqField.UserForceClose = 0;

	// Limit order
	/*
	reqField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	reqField.LimitPrice = 1000;// your price
	reqField.TimeCondition = THOST_FTDC_TC_GFD;// valid today;
	*/

	// Condition order
	/*
	reqField.ContingentCondition = THOST_FTDC_CC_Immediately;
	reqField.StopPrice = 2000; // trigger when price falls or rises this price
	reqField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	reqField.LimitPrice = 1000;
	reqField.TimeCondition = THOST_FTDC_TC_GFD;
	*/

	// Market order
	reqField.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	reqField.LimitPrice = 0;
	reqField.TimeCondition = THOST_FTDC_TC_IOC;
	reqField.ContingentCondition = THOST_FTDC_CC_Immediately;

	for (const auto& instrumentId : m_instrumentIds)
	{
		strcpy_s(reqField.InstrumentID, Util::WStringToString(instrumentId).c_str());
		CtpTradeSys::Get()->InsertOrder(&reqField);
	}
}
