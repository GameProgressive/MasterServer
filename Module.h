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
#ifndef _MASTERSERVER_MODULE_H
#define _MASTERSERVER_MODULE_H

#include <MasterServerMDK.h>

// Include pthreads/WINAPI threading
#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#else
	#include <pthread.h>
#endif

#include <string>
#include <map>

// MAX_PATH in Windows
#define MAX_MODULENAME 255

// Callbacks and typedef
typedef int (* Module_EntryPoint)(void*);
typedef bool (* Module_ConfigFunction)(ModuleConfigMap);

/*
	This class rappresents a dynamic module that will
	be loaded into RetroSpy Main Server
*/
class CModule
{
public:
	CModule();
	~CModule();

	/*
		Function: Load
		Descrption: Load a dynamic module
		Parameters: 
			name => Name of the module to be loaded (without .dll/.so)
		
		Return: true if the library is loaded
	*/
	bool Load(const char *name);
	
	/*
		Function: Load
		Descrption: Load a dynamic module
		Parameters: 
			name => Name of the module to be loaded (without .dll/.so)
			cfg => A map to the configuration parameters
		
		Return: true if the library is loaded
	*/
	bool Load(const char *name, ModuleConfigMap cfg);

	/*
		Function: Start
		Description: Starts the module
	*/
	void Start();

	/*
		Function: Stop
		Description: Stop the module
	*/
	void Stop();

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
		Function: GetName
		Description: Get the thread name
		Return: the thread name
	*/
	const char *GetName();
	
	/*
		Function: GetDatabaseStatus
		Description: Get the database status
		Return: the database status
	*/
	const char *GetDatabaseStatus();

	/*
		Function: GetExitCode
		Description: Get the exit code of the thread
		Return: the exit code
	*/
	unsigned long GetExitCode();

private:
	char m_szName[MAX_MODULENAME+1];

	Module_EntryPoint m_cbMain;
	Module_ConfigFunction m_cbConfig;

	ModuleMain m_module;

	bool m_bRunning;

#ifdef _WIN32
	HANDLE m_handle;
	DWORD m_threadID;
	HMODULE m_lpThread;
#else
	pthread_t m_threadID;
	void *m_lpThread;
#endif

	unsigned long m_exitCode;

	mdk_mysql m_connection;
	bool m_database_disabled;

	void UpdateThreadInformation();
};

#endif
