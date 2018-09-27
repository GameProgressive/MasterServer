/*
    This file is part of MasterServer.

    MasterServer is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MasterServer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with MasterServer.  If not, see <http://www.gnu.org/licenses/>.
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
	m_szName[0] = '\0';
	m_lpServer = NULL;
	m_cbMain = NULL;
	m_lpDatabase = NULL;
	m_lpDLL = NULL;
}

CModule::~CModule()
{
	Stop();
	
	// Free Dynamic Library
	if (m_lpDLL)
	{
#ifdef _WIN32
		FreeLibrary(m_lpDLL);
#else
		dlclose(m_lpDLL);
#endif
	}
}

bool CModule::Load(const char *name, ModuleConfigMap cfg)
{
	m_cfg = cfg;
	
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
	m_lpDLL = LoadLibrary(dllName.c_str());
#else
	m_lpDLL = dlopen(dllName.c_str(), RTLD_LAZY);
#endif

	if (!m_lpDLL)
	{
		LOG_ERROR("Module", "Cannot find library %s!", m_szName);
		return false;
	}

	// Load the functions
#ifdef _WIN32
	m_cbMain = (Module_EntryPoint)GetProcAddress(m_lpDLL, "MDKModule");
#else
	m_cbMain = (Module_EntryPoint)dlsym(m_lpDLL, "MDKModule");
#endif

	if (!m_cbMain)
	{
		LOG_ERROR("Module", "Invalid library %s!", m_szName);
		return false;
	}
	
	m_lpServer = m_cbMain();

	return true;
}

bool CModule::IsRunning()
{
	if (m_lpServer)
		return m_lpServer->IsRunning();
	
	return false;
}

bool CModule::Start(CDatabase* db)
{
	int port = -1;
	const char *ip = NULL;
	
	// Retrive bind ip and bind port
	{
		ModuleConfigMap::iterator it = m_cfg.find("Port");

		if (it != m_cfg.end())
			port = atoi(it->second.c_str());

		it = m_cfg.find("BindIP");

		if (it == m_cfg.end())
			ip = (char*)CConfig::GetDefaultIP();
		else
			ip = (char*)it->second.c_str();
	}
	
	// Connect to MySQL server
	if (CConfig::IsDatabaseEnabled())
	{
		bool database_disabled = false;
		ModuleConfigMap::iterator it = m_cfg.find("DisableMySQL");
		
		m_lpDatabase = db;
		
		if (it != m_cfg.end())
		{
			if (it->second.compare("1") == 0)
			{
				database_disabled = true;
			}
		}
		
		if (database_disabled == false && m_lpDatabase == NULL)
		{
			m_lpDatabase = new CDatabase();
			
			if (CConfig::GetDatabaseType() == DATABASE_TYPE_MARIADB)
			{
				if (CConfig::GetDatabaseSocket()[0] != '\0')
				{
					if (m_lpDatabase->Connect(CConfig::GetDatabaseType(), CConfig::GetDatabaseSocket(), -1, CConfig::GetDatabaseUsername(), CConfig::GetDatabaseName(), CConfig::GetDatabasePassword()))
						return false;
				}
				else
				{
					if (!m_lpDatabase->Connect(CConfig::GetDatabaseType(), CConfig::GetDatabaseHost(), CConfig::GetDatabasePort(), CConfig::GetDatabaseUsername(), CConfig::GetDatabaseName(), CConfig::GetDatabasePassword()))
						return false;
				}
			}
		}
	}
	
	return m_lpServer->Start(ip, port, m_lpDatabase, m_cfg);
}

const char *CModule::GetDatabaseStatus()
{
	if (!m_lpDatabase)
		return "Disabled";
	
	return m_lpDatabase->IsConnected() ? "Connected " : "Disconnected";
}

void CModule::Stop()
{
	if (m_lpServer)
	{
		m_lpServer->Stop();
		delete m_lpServer;
		m_lpServer = NULL;
	}
	
	// Close MySQL connection
	if (m_lpDatabase && m_lpDatabase->GetDatabaseType() == DATABASE_TYPE_MARIADB)
	{
		m_lpDatabase->Disconnect();
		delete m_lpDatabase;
	}
	
	m_lpDatabase = NULL;
}

int CModule::GetExitCode()
{
	if (m_lpServer == NULL)
		return ERROR_NONE;
	
	return m_lpServer->GetExitCode();
}