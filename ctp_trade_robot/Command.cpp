#include "stdafx.h"
#include "Command.h"

#include "CTPApp.h"

Command::Command()
{
}


Command::~Command()
{
}

void TerminateCommand::Execute()
{
	CtpApp::Get()->SetTerminating();
}
