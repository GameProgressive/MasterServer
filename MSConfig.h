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
