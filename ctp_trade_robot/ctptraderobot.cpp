// ctptraderobot.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "CtpTradeManager.h"

int main()
{
	CtpTradeRobot tradeRobot;

	tradeRobot.TradeInCTP();
	return 1;

	if (!tradeRobot.DownloadMarketData())
	{
		system("pause");
		return -1;
	}

	if (!tradeRobot.ApplyStrategy())
	{
		system("pause");
		return -1;
	}

	if (!tradeRobot.TradeInCTP())
	{
		system("pause");
		return -1;
	}

	system("pause");

    return 0;
}

