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

#include <stdio.h>

#ifndef _WIN32
#include <dlfcn.h>
#include <pthread.h>
#include <signal.h>
#endif

CModule::CModule()
{
	m_cbConfig = NULL;
	m_cbMain = NULL;

	m_exitCode = 0;

	m_database_disabled = false;

	m_module.ip = NULL;
	m_module.port = 0;
	m_module.mysql = NULL;

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
	std::string dllName = ".";
	
#ifndef _WIN32
	dllName += "/lib";
#else
	dllName += "\\";
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
	m_cbMain = (Module_EntryPoint)GetProcAddress(m_lpThread, "MDKModule");
#else
	m_cbMain = (Module_EntryPoint)dlsym(m_lpThread, "MDKModule");
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
				if (Database::Connect(m_connection, CConfig::GetDatabaseSocket(), CConfig::GetDatabaseUsername(), CConfig::GetDatabaseName(), CConfig::GetDatabasePassword()))
					m_module.mysql = m_connection;
			else
				if (Database::Connect(m_connection, CConfig::GetDatabaseHost(), CConfig::GetDatabasePort(), CConfig::GetDatabaseUsername(), CConfig::GetDatabaseName(), CConfig::GetDatabasePassword()))
					m_module.mysql = m_connection;
				
		}
	}
	
#ifdef _WIN32
	StartThread((LPTHREAD_START_ROUTINE)m_cbMain, (void*)&m_module);
#else
	StartThread((void*(*)(void*))m_cbMain, (void*)&m_module);	
#endif
}

const char *CModule::GetDatabaseStatus()
{
	if (m_database_disabled)
		return "Disabled";
	
	return Database::IsConnected(m_connection) ? "Connected " : "Disconnected";
}

void CModule::Stop()
{
	StopThread();
	
	// Close MySQL connection
	Database::Disconnect(m_connection);
	m_connection = NULL;
}

const char *CModule::GetName()
{
	return m_szName;
}
