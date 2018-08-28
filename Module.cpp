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
#include "Module.h"
#include "MSConfig.h"

#include <stdio.h>
#include <string.h>

#include <MDK/Utility.h>

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

	m_module.ip = NULL;
	m_module.port = 0;
	m_module.db = NULL;

	m_database = NULL;
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
	strncpy(m_szName, name, sizeof(m_szName));
	m_szName[sizeof(m_szName) - 1] = '\0';

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

void CModule::Start(CDatabase* db)
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
	if (CConfig::IsDatabaseEnabled())
	{
		bool database_disabled = false;
		ModuleConfigMap::iterator it = m_module.cfg.find("DisableMySQL");
		
		m_module.db = NULL;
		
		if (it != m_module.cfg.end())
		{
			if (it->second.compare("1") == 0)
			{
				database_disabled = true;
			}
		}
		
		if (!database_disabled)
		{
			if (db != NULL)
				m_database = db;
			else
				m_database = new CDatabase();
			
			if (m_database->GetDatabaseType() == DATABASE_TYPE_MARIADB)
			{
				if (CConfig::GetDatabaseSocket()[0] != '\0')
				{
					if (m_database->Connect(CConfig::GetDatabaseType(), CConfig::GetDatabaseSocket(), -1, CConfig::GetDatabaseUsername(), CConfig::GetDatabaseName(), CConfig::GetDatabasePassword()))
						m_module.db = m_database;
				}
				else
				{
					if (m_database->Connect(CConfig::GetDatabaseType(), CConfig::GetDatabaseHost(), CConfig::GetDatabasePort(), CConfig::GetDatabaseUsername(), CConfig::GetDatabaseName(), CConfig::GetDatabasePassword()))
						m_module.db = m_database;
				}
			}
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
	if (!m_database)
		return "Disabled";
	
	return m_database->IsConnected() ? "Connected " : "Disconnected";
}

void CModule::Stop()
{
	StopThread();
	
	// Close MySQL connection
	if (m_database)
	{
		m_database->Disconnect();
		delete m_database;
	}
	
	m_database = NULL;
}
