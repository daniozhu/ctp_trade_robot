#include "stdafx.h"
#include "FileMonitor.h"

#include "Util.h"

////////////////////////////////////////////////////////
// FileMonitor
////////////////////////////////////////////////////////
FileMonitor::FileMonitor()
	:m_bKillThread(false)
{
}


FileMonitor::~FileMonitor()
{
	for (auto p : m_dirs)
	{
		delete p;
	}

	Kill();

	CloseHandle(m_hEvent);
}

void FileMonitor::Add(const FileChangeObserver * pObserver)
{
	const std::wstring filePath = pObserver->GetFilePath();
	std::wstring fileDir = Util::GetFileDir(filePath);

	// Check if the dir is already being watched or not
	auto iFound = std::find_if(m_dirs.begin(), m_dirs.end(), 
		[&fileDir](MonitorDirectory* pDir) {return fileDir.compare(pDir->GetDirPath())==0; }
	);
	
	MonitorDirectory* pTargetDir = nullptr;
	bool bNewDirAdded = false;
	if (iFound == m_dirs.end())
	{
		m_dirs.push_back(new MonitorDirectory(fileDir));
		pTargetDir = m_dirs.back();
		bNewDirAdded = true;
	}
	else
	{
		pTargetDir = *iFound;
	}

	pTargetDir->Add(pObserver);

	// if new dir added, let thread to refresh wait handles.
	if (bNewDirAdded)
		SetEvent(m_hEvent);
}

void FileMonitor::Remove(const FileChangeObserver * pObserver)
{
}

void FileMonitor::Start()
{
	m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_thread = std::thread(&FileMonitor::WatchFileChange, std::ref(*this));
}

void FileMonitor::Kill()
{
	m_bKillThread = true;
	::SetEvent(m_hEvent);

	m_thread.join();
}

FileMonitor * FileMonitor::Get()
{
	static FileMonitor g_fileMonitor;
	return &g_fileMonitor;
}

void FileMonitor::WatchFileChange()
{
	while (!m_bKillThread)
	{
		RefreshHandles();

		DWORD dwStatus = ::WaitForMultipleObjects(m_nHandles, m_handles, FALSE, INFINITE);
		if (dwStatus == WAIT_OBJECT_0)
		{
			continue;
		}
		else if (dwStatus > WAIT_OBJECT_0 + m_nHandles)
		{
			continue;
		}
		else
		{
			int index = dwStatus - WAIT_OBJECT_0;
			HANDLE handle = m_handles[index];
			const MonitorDirectory* pDir = GetDirectoryFromHandle(handle);
			if (pDir != nullptr)
			{
				// Get a copy of the files in case main thread is adding file
				std::vector<FileEntry*> files = pDir->GetFiles();
				for (auto pFile : files)
				{
					if (pFile != nullptr && pFile->IsModified()) {
						pFile->NotifyChange();
						pFile->UpdateLastModifiedTime();
					}
				}
			}
			// Continue to monitor 
			::FindNextChangeNotification(handle);
		}
	}
}

void FileMonitor::RefreshHandles()
{
	m_nHandles = 0;
	m_handles[m_nHandles++] = m_hEvent;

	for (const auto p : m_dirs)
	{
		if (p->GetHandle() != INVALID_HANDLE_VALUE)
		{
			m_handles[m_nHandles++] = p->GetHandle();
		}
	}
}

const MonitorDirectory * FileMonitor::GetDirectoryFromHandle(HANDLE handle)
{
	auto iFound = std::find_if(m_dirs.begin(), m_dirs.end(), 
		[&](MonitorDirectory* dir) {return dir->GetHandle() == handle; });

	return iFound == m_dirs.end() ? nullptr : *iFound;
}

///////////////////////////////////////////////
// FileChangeObserver
///////////////////////////////////////////////
FileChangeObserver::FileChangeObserver(const std::wstring & fileName)
	:m_filePath(fileName)
{
}


///////////////////////////////////////////////
// CmdFileChangeObserver
////////////////////////////////////////////////
CmdFileChangeObserver::CmdFileChangeObserver(const std::wstring & filePath)
	: FileChangeObserver(filePath)
{
}

void CmdFileChangeObserver::OnEvent()
{
	// TODO
	std::cout << "File changed" << std::endl;
}

////////////////////////////////////////////////////////////
// MonitorDirectory
////////////////////////////////////////////////////////////
MonitorDirectory::MonitorDirectory(const std::wstring & dirPath)
	: m_dirPath(dirPath)
	, m_handle(INVALID_HANDLE_VALUE)
{
	m_handle = ::FindFirstChangeNotification(dirPath.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE);
	m_files.clear();
}

MonitorDirectory::~MonitorDirectory()
{
	if (m_handle != INVALID_HANDLE_VALUE)
	{
		::FindCloseChangeNotification(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}

	for (auto p : m_files) 
	{
		delete p;
	}

	m_files.clear();
}

void MonitorDirectory::Remove(const FileChangeObserver* pObserver)
{
}

void MonitorDirectory::Add(const FileChangeObserver* pObserver)
{
	if (pObserver == nullptr)
		return;

	const std::wstring& filePath = pObserver->GetFilePath();
	auto iFoundFile = std::find_if(m_files.begin(), m_files.end(), 
		[&](const FileEntry* file) {return file->GetFilePath().compare(filePath) == 0; }
	);
	
	FileEntry* pFileEntry = nullptr;
	if(iFoundFile == m_files.end())
	{
		m_files.push_back(new FileEntry(pObserver->GetFilePath()));
		pFileEntry = m_files.back();
	}
	else
	{
		pFileEntry = *iFoundFile;
	}

	pFileEntry->AddObserver(pObserver);
}

FileEntry::FileEntry(const std::wstring& filePath)
	: m_filePath(filePath)
{
	UpdateLastModifiedTime();
}

FileEntry::~FileEntry()
{
}

///////////////////////////////////////////////////
// FileEntry
///////////////////////////////////////////////////
bool FileEntry::IsModified() const
{
	FILETIME newTime = Util::GetFileLastModifiedTime(m_filePath);
	return ::CompareFileTime(&m_lastModTime, &newTime) != 0;
}

void FileEntry::AddObserver(const FileChangeObserver * pObserver)
{
	if(pObserver != nullptr)
		m_pObservers.push_back(const_cast<FileChangeObserver*>(pObserver));
}

void FileEntry::RemoveObserver(const FileChangeObserver * pObserver)
{
	if (pObserver)
	{

	}
}

void FileEntry::NotifyChange() const
{
	for (auto p : m_pObservers)
	{
		if (p != nullptr)
			p->OnEvent();
	}
}

void FileEntry::UpdateLastModifiedTime()
{
	m_lastModTime = Util::GetFileLastModifiedTime(m_filePath);
}
