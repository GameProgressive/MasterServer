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
#include "Module.h"
#include "MSConfig.h"
#include "Database.h"

#include <stdio.h>

#ifndef _WIN32
#include <dlfcn.h>
#include <pthread.h>
#include <signal.h>
#endif

CModule::CModule()
{
	m_bRunning = false;

	m_threadID = 0;
	m_lpThread = NULL;
	
	m_cbConfig = NULL;
	m_cbMain = NULL;

#ifdef _WIN32
	m_handle = NULL;
#endif

	m_exitCode = 0;

	m_database_disabled = false;

	Database::Init(&m_connection);
}

CModule::~CModule()
{
	Stop();
	
	// Free Dynamic Library
	if (m_lpThread)
	{
#ifdef _WIN32
		FreeLibrary(m_lpThread);
#else
		dlclose(m_lpThread);
#endif
	}
}

bool CModule::Load(const char *name, ModuleConfigMap cfg)
{
	m_module.cfg = cfg;

	return Load(name);
}


bool CModule::Load(const char *name)
{
	std::string dllName = "";
	
#ifndef _WIN32
	dllName += "lib";
#endif

	dllName += name;

#ifdef _WIN32
	dllName += ".dll";
#else
	dllName += ".so";
#endif
	
	// Save the name
	strncpy_s(m_szName, sizeof(m_szName), name, MAX_MODULENAME);

	// Load the Dynamic library
#ifdef _WIN32
	m_lpThread = LoadLibrary(dllName.c_str());
#else
	m_lpThread = dlopen(dllName.c_str(), RTLD_LAZY);
#endif

	if (!m_lpThread)
	{
		LOG_ERROR("Module", "Cannot find library %s!", m_szName);
		return false;
	}

	// Load the functions
#ifdef _WIN32
	m_cbMain = (Module_EntryPoint)GetProcAddress(m_lpThread, "RetroSpyMain");
#else
	m_cbMain = (Module_EntryPoint)dlsym(m_lpThread, "RetroSpyMain");
#endif

	if (!m_cbMain)
	{
		LOG_ERROR("Module", "Invalid library %s!", m_szName);
		return false;
	}

	return true;
}

void CModule::Start()
{
	// Retrive bind ip and bind port
	{
		ModuleConfigMap::iterator it = m_module.cfg.find("Port");

		if (it == m_module.cfg.end())
			m_module.port = -1;
		else
			m_module.port = atoi(it->second.c_str());

		it = m_module.cfg.find("BindIP");

		if (it == m_module.cfg.end())
			m_module.ip = (char*)CConfig::GetDefaultIP();
		else
			m_module.ip = (char*)it->second.c_str();
	}
	
	// Connect to MySQL server
	{
		ModuleConfigMap::iterator it = m_module.cfg.find("DisableMySQL");
		
		m_module.mysql = NULL;
		
		if (it != m_module.cfg.end())
		{
			if (it->second.compare("1") == 0)
			{
				m_database_disabled = true;
			}
		}
		
		if (!m_database_disabled)
		{
			if (CConfig::GetDatabaseSocket())
				if (Database::Connect(&m_connection, CConfig::GetDatabaseSocket(), CConfig::GetDatabaseUsername(), CConfig::GetDatabaseName(), CConfig::GetDatabasePassword()))
					m_module.mysql = &m_connection;
			else
				if (Database::Connect(&m_connection, CConfig::GetDatabaseHost(), CConfig::GetDatabasePort(), CConfig::GetDatabaseUsername(), CConfig::GetDatabaseName(), CConfig::GetDatabasePassword()))
					m_module.mysql = &m_connection;
				
		}
	}
	
	// Create the module thread
#ifdef _WIN32
	m_handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)m_cbMain, (void*)&m_module, 0, &m_threadID);

	if (m_handle)
		m_bRunning = true;
#else
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	if (!pthread_create(&m_threadID, &attr,(void*(*)(void*)) m_cbMain, (void*)&m_module))
		m_bRunning = true;
#endif

	UpdateThreadInformation();
}

const char *CModule::GetDatabaseStatus()
{
	if (m_database_disabled)
		return "Disabled";
	
	return Database::IsConnected(&m_connection) ? "Connected " : "Disconnected";
}

void CModule::Stop()
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
	
	// Close MySQL connection
	Database::Disconnect(&m_connection);
}

#ifdef _WIN32
DWORD CModule::getThreadID()
#else
pthread_t CModule::getThreadID()
#endif
{
	return m_threadID;
}

bool CModule::IsRunning()
{
	UpdateThreadInformation();

	return m_bRunning;
}

const char *CModule::GetName()
{
	return m_szName;
}

unsigned long CModule::GetExitCode()
{
	return m_exitCode;
}

void CModule::UpdateThreadInformation()
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
#error "REQUIRE POSIX PORTING"
#endif
}