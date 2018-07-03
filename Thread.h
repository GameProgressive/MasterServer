/*
	Licensed to the Apache Software Foundation (ASF) under one
	or more contributor license agreements.  See the NOTICE file
	distributed with this work for additional information
	regarding copyright ownership.  The ASF licenses this file
	to you under the Apache License, Version 2.0 (the
	"License"); you may not use this file except in compliance
	with the License.  You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing,
	software distributed under the License is distributed on an
	"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
	KIND, either express or implied.  See the License for the
	specific language governing permissions and limitations
	under the License.
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
	void StartThread(void*(*)(void*) m_cbMain, void* m_args);
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
