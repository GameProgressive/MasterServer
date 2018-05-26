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
	static const char *GetDatabaseSocket();
	static const char *GetDefaultIP();
	static const char *GetDatabaseHost();
	static const char *GetDatabasePassword();
	static int GetDatabasePort();
	static const char *GetDatabaseName();
	static const char *GetDatabaseUsername();
	
	static bool Load(CModuleManager *mngr, const char *name);
private:
	static int m_DBPort;
	static char m_szDBHost[MAX_INI_BUFFER+1];
	static char m_szDBUser[MAX_INI_BUFFER+1];
	static char m_szDBPass[MAX_INI_BUFFER+1];
	static char m_szDBSock[MAX_INI_BUFFER+1];
	static char m_szDBName[MAX_INI_BUFFER+1];
	static char m_szDIP[MAX_INI_BUFFER+1];

};

#endif
