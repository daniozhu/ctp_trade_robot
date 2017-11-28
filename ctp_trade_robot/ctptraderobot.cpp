// ctptraderobot.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "CMarketDataDownLoader.h"

int main()
{
	std::wstring sURL = LR"(http://stock.finance.sina.com.cn/futures/api/json.php/InnerFuturesService.getInnerFuturesDailyKLine?symbol=RB1710)";
	std::wstring sLocalPath = LR"(d:\\trade_data\rb1710.txt)";

	bool bRet = CMarketDataDownLoader::Get()->Download(sURL, sLocalPath);

    return 0;
}

