#pragma once

#include <string>
#include <map>
#include <vector>
#include <thread>
#include <mutex>

class FileChangeObserver;

class FileEntry
{
public:
	FileEntry(const std::wstring& filePath);
	~FileEntry();

	bool	IsModified() const;
	bool	IsNotUsed() const { return m_pObservers.empty(); }

	void	AddObserver(const FileChangeObserver* pObserver);
	void	RemoveObserver(const FileChangeObserver* pObserver);

	const std::wstring& GetFilePath() const { return m_filePath; }

	void NotifyChange() const;
	void UpdateLastModifiedTime();

private:
	std::vector<FileChangeObserver*>	m_pObservers;
	FILETIME							m_lastModTime;

	std::wstring						m_filePath;
};

class MonitorDirectory
{
public:
	MonitorDirectory(const std::wstring& dirPath);
	~MonitorDirectory();

	void Remove(const FileChangeObserver* pObserver);
	void Add(const FileChangeObserver* pObserver);
	HANDLE GetHandle()const { return m_handle; }

	const std::wstring& GetDirPath()const { return m_dirPath; }

	const std::vector<FileEntry*>& GetFiles() const { return m_files; }

private:
	std::vector<FileEntry*>  m_files;
	std::wstring			m_dirPath;
	HANDLE					m_handle;
};


class FileMonitor
{
public:
	void Add(const FileChangeObserver* pObserver);
	void Remove(const FileChangeObserver* pObserver);

	void Start();
	void Kill();

	static FileMonitor* Get();

private:
	FileMonitor();
	~FileMonitor();

	// Thread function
	void WatchFileChange();

	void RefreshHandles();
	const MonitorDirectory* GetDirectoryFromHandle(HANDLE handle);

private:
	std::vector<MonitorDirectory*>  m_dirs;

	HANDLE				m_hEvent;
	HANDLE				m_handles[MAX_PATH];
	int					m_nHandles;
	std::thread			m_thread;
	volatile bool		m_bKillThread;
//	std::mutex			m_mutex;
};

class FileChangeObserver
{
public:
	FileChangeObserver(const std::wstring& fileName);

	const std::wstring& GetFilePath() const { return m_filePath; }

	virtual void OnEvent() = 0;

private:
	std::wstring		m_filePath;
};

class CmdFileChangeObserver : public FileChangeObserver
{
public:
	CmdFileChangeObserver(const std::wstring& filePath);

	virtual void OnEvent() override;
};

