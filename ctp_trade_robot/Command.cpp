#include "stdafx.h"
#include "Command.h"

#include "CTPApp.h"


#include "CtpMarketDataSys.h"
#include "CtpTradeSys.h"
#include "FileMonitor.h"

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
