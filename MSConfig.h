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
#ifndef _MASTERSERVER_CONFIG_H
#define _MASTERSERVER_CONFIG_H

#include "ModuleManager.h"

#define MAX_INI_BUFFER 100

class CConfig
{
public:
	inline static const char *GetDatabaseSocket() { return m_szDBSock; }
	inline static const char *GetDefaultIP() { return m_szDBIP; }
	inline static const char *GetDatabaseHost() { return m_szDBHost; }
	inline static const char *GetDatabasePassword() { return m_szDBPass; }
	inline static int GetDatabasePort() { return m_DBPort; }
	inline static const char *GetDatabaseName() { return m_szDBName; }
	inline static const char *GetDatabaseUsername() { return m_szDBUser; }
	inline static bool IsDatabaseEnabled() { return m_bDBEnabled; }
	inline static EDatabaseType GetDatabaseType() { return m_eDatabaseType; }
	
	static bool Load(CModuleManager *mngr, const char *name);
private:
	static int m_DBPort;
	static char m_szDBHost[MAX_INI_BUFFER+1];
	static char m_szDBUser[MAX_INI_BUFFER+1];
	static char m_szDBPass[MAX_INI_BUFFER+1];
	static char m_szDBSock[MAX_INI_BUFFER+1];
	static char m_szDBName[MAX_INI_BUFFER+1];
	static char m_szDBIP[MAX_INI_BUFFER+1];
	static bool m_bDBEnabled;
	static EDatabaseType m_eDatabaseType;
	
};

#endif
