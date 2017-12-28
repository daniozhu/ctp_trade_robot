#pragma once

#include <memory>

class Command
{
public:
	Command();
	~Command();

	virtual void Execute() = 0;
};

using CommandPtr = std::shared_ptr<Command>;

class TerminateCommand : public Command
{
public:
	virtual void Execute() override;
};
