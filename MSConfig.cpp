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
#include "MSConfig.h"

#include "INIReader.h"

#include <stdio.h>

#include <string.h>

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
		char dbType[MAX_INI_BUFFER+1];
		dbType[0] = '\0';
		
		m_DBPort = reader.GetInteger("Database", "Port", 3306);
		if (m_DBPort < 0 || m_DBPort > 65535)
			m_DBPort = 3306;

		strncpy(m_szDBPass, reader.Get("Database", "Password", "").c_str(), sizeof(m_szDBPass));
		m_szDBPass[sizeof(m_szDBPass) - 1] = '\0';
		strncpy(m_szDBUser, reader.Get("Database", "Username", "masterserver").c_str(), sizeof(m_szDBUser));
		m_szDBUser[sizeof(m_szDBUser) - 1] = '\0';
		strncpy(m_szDBHost, reader.Get("Database", "Host", "localhost").c_str(), sizeof(m_szDBHost));
		m_szDBHost[sizeof(m_szDBHost) - 1] = '\0';
		strncpy(m_szDBSock, reader.Get("Database", "Socket", "").c_str(), sizeof(m_szDBSock));
		m_szDBSock[sizeof(m_szDBSock) - 1] = '\0';

		strncpy(dbType, reader.Get("Database", "Type", "MariaDB").c_str(), sizeof(dbType));
		dbType[sizeof(dbType) - 1] = '\0';

		if (strcmp("MariaDB", dbType) == 0)
			m_eDatabaseType = DATABASE_TYPE_MARIADB;
		else if (strcmp("SQLite", dbType) == 0)
			m_eDatabaseType = DATABASE_TYPE_SQLITE;

		if (m_eDatabaseType == DATABASE_TYPE_MARIADB)
			strncpy(m_szDBName, reader.Get("Database", "Name", "masterserver").c_str(), sizeof(m_szDBName));
		else
			strncpy(m_szDBName, reader.Get("Database", "Name", "masterserver.db").c_str(), sizeof(m_szDBName));

		m_szDBName[sizeof(m_szDBName) - 1] = '\0';
		
		strncpy(m_szDBIP, reader.Get("Server", "DefaultIP", "localhost").c_str(), sizeof(m_szDBIP));
		m_szDBIP[sizeof(m_szDBIP) - 1] = '\0';
		m_bDBEnabled = true;
	}
	else
		m_bDBEnabled = false;
	
	// Load the modules
	while (bC)
	{
		char mn[15];
		snprintf(mn, sizeof(mn), "%u", i);

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
char CConfig::m_szDBIP[MAX_INI_BUFFER+1] = {0};
bool CConfig::m_bDBEnabled = false;

EDatabaseType CConfig::m_eDatabaseType = DATABASE_TYPE_MARIADB;
