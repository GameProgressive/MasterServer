/*
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _MASTERSERVER_THREAD_H
#define _MASTERSERVER_THREAD_H

// Include pthreads/WINAPI threading
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#else
	#include <pthread.h>
#endif

class CThread
{
public:
	CThread();
	~CThread();
	
	/*
		Function: StartThread
		Description: Starts the thread
		Parameters:
			m_cbMain => The main callback to be called
			m_args => Arguments to be passed in the main callback
	*/
#ifdef _WIN32
	void StartThread(LPTHREAD_START_ROUTINE m_cbMain, void* m_args);
#else
	void StartThread(void*(*m_cb_main)(void*), void* m_args);
#endif
	
	/*
		Function: StopThread
		Description: Stop the thread
	*/
	void StopThread();

	/*
		Function: IsRunning
		Descrption: Checks if the module is running
		Return: true if the module is running, otherwise false
	*/
	bool IsRunning();

	/*
		Function: GetThreadID
		Description: Get the thread ID
		Return: The thread ID
	*/
#ifdef _WIN32
	DWORD getThreadID();
#else
	pthread_t getThreadID();
#endif

	/*
		Function: GetExitCode
		Description: Get the exit code of the thread
		Return: the exit code
	*/
	unsigned long GetExitCode();

protected:
	void UpdateThreadInformation();

#ifdef _WIN32
	HANDLE m_handle;
	DWORD m_threadID;
	HMODULE m_lpThread;
#else
	pthread_t m_threadID;
	void *m_lpThread;
#endif

	unsigned long m_exitCode;
	bool m_bRunning;
};


#endif
