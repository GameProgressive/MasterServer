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
#include "MSConfig.h"

#include "INIReader.h"

#include <MDK/MasterServerMDK.h>
#include <stdio.h>

const char *CConfig::GetDatabaseName()
{
	return m_szDBName;
}

int CConfig::GetDatabasePort()
{
	return m_DBPort;
}

const char *CConfig::GetDatabaseUsername()
{
	return m_szDBUser;
}

const char *CConfig::GetDatabasePassword()
{
	return m_szDBPass;
}

const char *CConfig::GetDatabaseHost()
{
	return m_szDBHost;
}

const char *CConfig::GetDefaultIP()
{
	return m_szDIP;
}

const char *CConfig::GetDatabaseSocket()
{
	return m_szDBSock;
}

bool CConfig::IsDatabaseEnabled()
{
	return m_dbenb;
}

bool CConfig::Load(CModuleManager *mngr, const char *name)
{
	// Create an INI instance
	INIReader reader(name);

	size_t i = 0, k = 0;

	bool bC = true;

	std::string str = "";

	// Load the INI
	if (reader.ParseError() < 0)
		return false;

	// Load the Database section
	if (reader.GetInteger("Database", "Enabled", 1) == 1)
	{
		m_DBPort = reader.GetInteger("Database", "Port", 3306);
		if (m_DBPort < 0 || m_DBPort > 65535)
			m_DBPort = 3306;

		strncpy_s(m_szDBName, sizeof(m_szDBName), reader.Get("Database", "Name", "masterserver").c_str(), MAX_INI_BUFFER);
		strncpy_s(m_szDBPass, sizeof(m_szDBPass), reader.Get("Database", "Password", "").c_str(), MAX_INI_BUFFER);
		strncpy_s(m_szDBUser, sizeof(m_szDBUser), reader.Get("Database", "Username", "masterserver").c_str(), MAX_INI_BUFFER);
		strncpy_s(m_szDBHost, sizeof(m_szDBHost), reader.Get("Database", "Host", "localhost").c_str(), MAX_INI_BUFFER);
		strncpy_s(m_szDBSock, sizeof(m_szDBSock), reader.Get("Database", "Socket", "").c_str(), MAX_INI_BUFFER);

		strncpy_s(m_szDIP, sizeof(m_szDIP), reader.Get("Server", "DefaultIP", "localhost").c_str(), MAX_INI_BUFFER);
		m_dbenb = true;
	}
	else
		m_dbenb = false;
	
	// Load the modules
	while (bC)
	{
		char mn[15];
		_snprintf_s(mn, sizeof(mn), 15, "%u", i);

		// Try to get the module name
		str = reader.Get("Modules", std::string(mn), "NOT_FOUND");
		
		if (str.compare("NOT_FOUND") == 0)
		{
			
			bC = false; // This is the last module, exit
		}
		else
		{
			// Get the fields of the module section
			std::set<std::string> set = reader.GetFields(str);

			if (set.size() < 1)
				// Just load without the configuration if we don't have any
				mngr->LoadMSModule(str.c_str());
			else
			{
				ModuleConfigMap map;

				for (k = 0; k < set.size(); k++)
				{
					// Get the string on the specific position
					std::set<std::string>::iterator it = set.begin();
					std::advance(it, k);

					// Set the map value to the field
					map[*it] = reader.Get(str, *it, "");
				}

				// Load the module with the config
				mngr->LoadMSModule(str.c_str(), map);
			}
		}

		// Advance to the next load
		i++;
	}

	return true;
}

int CConfig::m_DBPort = 0;
char CConfig::m_szDBHost[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDBUser[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDBPass[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDBSock[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDBName[MAX_INI_BUFFER+1] = {0};
char CConfig::m_szDIP[MAX_INI_BUFFER+1] = {0};
bool CConfig::m_dbenb = false;
