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
	virtual ~TerminateCommand() = default;
	virtual void Execute() override;
};

class OpenPositionCommand : public Command
{
public:
	OpenPositionCommand(const std::vector<std::wstring>& instrumentIds);
	~OpenPositionCommand() = default;
	virtual void Execute() override;

private:
	std::vector<std::wstring>		m_instrumentIds;
};
