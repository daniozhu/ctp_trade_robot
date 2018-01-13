// ctptraderobot.cpp
//

#include "stdafx.h"

#include "CtpTradeSys.h"
#include "CtpMarketDataSys.h"
#include "MessageQueue.h"
#include "Command.h"
#include "CTPApp.h"
#include "FileMonitor.h"


int main()
{
	// Start trade system
	CtpTradeSys::Get()->Start();

	// Start market data system
	//CtpMarketDataSys::Get()->Start();

	// Start file monitor for command file
	std::unique_ptr<FileChangeObserver> spObserver{ new CmdFileChangeObserver() };
	FileMonitor::Get()->Add(spObserver.get());
	FileMonitor::Get()->Start();

	// Get command from message queue and execute
	while (!CtpApp::Get()->IsTerminating())
	{
		CommandPtr spCmd = MessageQueue<CommandPtr>::Get()->Pop();
		spCmd->Execute();
	}

	system("pause");

    return 0;
}

