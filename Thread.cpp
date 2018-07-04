#include "Thread.h"
#include "MSConfig.h"

CThread::CThread()
{
#ifdef _WIN32
	m_handle = NULL;
#endif
	m_threadID = 0;
	m_lpThread = NULL;
	m_bRunning = false;
}

CThread::~CThread()
{
	if (m_bRunning)
		StopThread();
	
	
}

#ifdef _WIN32
void CThread::StartThread(LPTHREAD_START_ROUTINE m_cbMain, void* m_args)
#else
void CThread::StartThread(void*(*)(void*) m_cbMain, void* m_args)
#endif
{
	// Create the module thread
#ifdef _WIN32
	m_handle = CreateThread(0, 0, m_cbMain, m_args, 0, &m_threadID);

	if (m_handle)
		m_bRunning = true;
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	if (!pthread_create(&m_threadID, &attr, m_cbMain, m_args))
		m_bRunning = true;
#endif

	UpdateThreadInformation();	
}

void CThread::StopThread()
{
	UpdateThreadInformation();

	if (!m_bRunning)
		return;

	m_bRunning = false;

	// Terminate the thread
#ifdef _WIN32
	TerminateThread(m_handle, 0);
	m_handle = NULL; //SET THIS TO NULL OTHERWISE THE APP WILL CRASH
#else
	pthread_kill(m_threadID, SIGKILL);
#endif
	m_threadID = 0;	
}

#ifdef _WIN32
DWORD CThread::getThreadID()
#else
pthread_t CThread::getThreadID()
#endif
{
	return m_threadID;
}

bool CThread::IsRunning()
{
	UpdateThreadInformation();

	return m_bRunning;
}

unsigned long CThread::GetExitCode()
{
	return m_exitCode;
}

void CThread::UpdateThreadInformation()
{
	if (!m_handle)
		return;

#ifdef _WIN32
	if (!GetExitCodeThread(m_handle, &m_exitCode))
		return;

	if (m_exitCode == ERROR_STILL_ALIVE)
		m_bRunning = true;
	else
		m_bRunning = false;

#else
	m_bRunning = true;
	m_exitCode = ERROR_LINUX_UNSUPPORTED_RESERVED;
#endif
}
