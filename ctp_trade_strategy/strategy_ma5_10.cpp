#include "stdafx.h"
#include "strategy_ma5_10.h"
#include "ILog.h"

#include "../json_cpp/include/json.h"

#define LOG_MESSAGE(pLog, level, msg)\
if(pLog)\
	pLog->Write(level, msg);

namespace
{
	using MA_Data = std::pair<std::string /*date*/, double /*MA value*/>;
	using  MA_History_Data = std::vector<MA_Data>;
	MA_History_Data GetMAHistoryData(const Json::Value& jsonRoot, int maDay, int daysBack)
	{
		const int size = jsonRoot.size();

		MA_History_Data historyData;

		for (int i = 0; i < daysBack; ++i)
		{
			double ma_xd_back = 0.0;

			for (int j = size - maDay - i; j < size - i; ++j) {
				ma_xd_back += ::atof(jsonRoot[j]["close"].asCString());
			}
			ma_xd_back = ma_xd_back / maDay;

			historyData.emplace_back(jsonRoot[size-1 - i]["date"].asString(), ma_xd_back);
		}

		return std::move(historyData);
	}
}

Strategy_MA5_10::Strategy_MA5_10(const StringMap & instrumentIdData, ILog* pLog)
	: m_instrumentIdData(instrumentIdData)
	, m_pLog(pLog)
{
}

Strategy_MA5_10::~Strategy_MA5_10()
{
}

bool Strategy_MA5_10::Process()
{
	Json::CharReaderBuilder builder = Json::CharReaderBuilder();
	builder["collectComments"] = false;

	JSONCPP_STRING errs;
	Json::Value value;

	// Caculate MA5 and MA10 for N days back
	const int nDaysBack = 2;

	for (const auto& idData : m_instrumentIdData)
	{
		const std::wstring& id = idData.first;
		const std::wstring& path = idData.second;

		LOG_MESSAGE(m_pLog, ILog::LogLevel::eNormal, L"Parsing json file -" + path);

		std::ifstream config_doc(idData.second, std::ifstream::binary);
		const bool bRet = Json::parseFromStream(builder, config_doc, &value, &errs);
		if (!bRet) 
		{
			LOG_MESSAGE(m_pLog, ILog::LogLevel::eError, L"Parse json file failed - " + path);
			continue;
		}

		if (value.size() < 11)
		{
			LOG_MESSAGE(m_pLog, ILog::LogLevel::eError, L"History data is too less, cannot apply MA5/MA10 strategry");
			continue;
		}

		// Note: the history vector holds the MA data from latest to old. 
		// e.g. the item 0 is yesterday's MA data, item 1 is the day before yesterday's MA data.
		const MA_History_Data ma_5_nd_back = GetMAHistoryData(value, 5, nDaysBack);

		// Caculate MA10 for N days back
		const MA_History_Data ma_10_nd_back = GetMAHistoryData(value, 10, nDaysBack);

		if (ma_5_nd_back.size() != ma_10_nd_back.size())
		{
			LOG_MESSAGE(m_pLog, ILog::LogLevel::eError, L"The MA5 and MA10 data is not matched in days");
			continue;
		}

#ifdef  _DEBUG
		// Dump the MA data to excel file for visually review
		std::ofstream MADataFile;
		MADataFile.open(L"..\\x64\\Debug\\" + id + L"_MAData.csv");
		
		MADataFile << "Date, " << "MA5, " << "MA10" << std::endl;

		for (int i = (int)ma_5_nd_back.size() - 1; i >=0 ; --i)
		{
			MADataFile << ma_5_nd_back[i].first << ", "
				<< ma_5_nd_back[i].second << ", "
				<< ma_10_nd_back[i].second << std::endl;
		}
		MADataFile.close();
#endif //  _DEBUG

		const bool bMA5GreaterThanMA10_1d_back = ma_5_nd_back[0].second - ma_10_nd_back[0].second > 0;
		const bool bMA5GreaterThanMA10_2d_back = ma_5_nd_back[1].second - ma_10_nd_back[1].second > 0;

		const bool bMA5Up_In_2ds = ma_5_nd_back[0].second - ma_5_nd_back[1].second > 0;
		const bool bMA10Up_In_2ds = ma_10_nd_back[0].second - ma_10_nd_back[1].second > 0;

		// Is golden cross?
		const bool bGodenCross = bMA5GreaterThanMA10_1d_back && 
								!bMA5GreaterThanMA10_2d_back && 
								bMA5Up_In_2ds && 
								bMA10Up_In_2ds;

		// Is dead cross ?
		const bool bDeadCross = !bMA5GreaterThanMA10_1d_back &&
								bMA5GreaterThanMA10_2d_back &&
								!bMA5Up_In_2ds &&
								!bMA10Up_In_2ds;

		if (bGodenCross)
		{
			TradeSuggestion suggestion;
			suggestion.InstrumentId = id;
			suggestion.IsBuy = true;
			suggestion.Confidence = TradeSuggestion::Level::eNormal;

			m_suggestions.emplace_back(std::move(suggestion));
		}

		if (bDeadCross)
		{
			TradeSuggestion suggestion;
			suggestion.InstrumentId = id;
			suggestion.IsBuy = false;
			suggestion.Confidence = TradeSuggestion::Level::eNormal;
			
			m_suggestions.emplace_back(std::move(suggestion));
		}
	}

	return true;
}


