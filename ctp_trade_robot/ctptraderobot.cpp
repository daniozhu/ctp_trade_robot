// ctptraderobot.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "CtpTradeManager.h"
#include "CtpMarketDataSys.h"
#include "MessageQueue.h"
#include "Command.h"
#include "CTPApp.h"

#include "FileMonitor.h"

#include <thread>

int main()
{
	std::unique_ptr<FileChangeObserver> spObserver{ new CmdFileChangeObserver(L"D:\\cmd.txt") };
	FileMonitor::Get()->Add(spObserver.get());
	FileMonitor::Get()->Start();

	// Start trade thread
	CtpTradeRobot tradeRobot;
	std::thread tradeThread{&CtpTradeRobot::Start, std::ref(tradeRobot)};

	// Start market thread
	CtpMarketDataSys marketSys;
	std::thread marketThread{&CtpMarketDataSys::Start, std::ref(marketSys)};

	// TODO: start file monitor thread

	// TODO: get command from message queue
	while (!CtpApp::Get()->IsTerminating())
	{
		CommandPtr spCmd = MessageQueue<CommandPtr>::Get()->Pop();
		spCmd->Execute();
	}

	tradeThread.join();
	marketThread.join();

//	system("pause");

    return 0;
}

