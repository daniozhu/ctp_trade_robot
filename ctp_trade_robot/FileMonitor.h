#pragma once

#include <string>

class FileChangeObserver;
class FileMonitorImp;

class FileMonitor
{
public:
	void Add(const FileChangeObserver* pObserver);

	void Start();
	void Kill();

	static FileMonitor* Get();

private:
	FileMonitor();
	~FileMonitor();

private:
	FileMonitorImp*		m_pImp;
};

class FileChangeObserver
{
public:
	FileChangeObserver(const std::wstring& fileName);

	const std::wstring& GetFilePath() const { return m_filePath; }

	virtual void OnChange() = 0;

private:
	std::wstring		m_filePath;
};

class CmdFileChangeObserver : public FileChangeObserver
{
public:
	CmdFileChangeObserver(const std::wstring& filePath);

	virtual void OnChange() override;
};

