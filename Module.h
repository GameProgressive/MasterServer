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
#ifndef _MASTERSERVER_MODULE_H
#define _MASTERSERVER_MODULE_H

#include <MDK/ModuleEntryPoint.h>

// MAX_PATH in Windows
#define MAX_MODULENAME 255

// Callbacks and typedef
typedef CThreadServer* (* Module_EntryPoint)(void);

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
		Parameters:
			db => A pointer to a current database connection, pass NULL to let the module create one by himself
	*/
	bool Start(CDatabase* db);

	/*
		Function: Stop
		Description: Stop the module
	*/
	void Stop();


	/*
		Function: GetName
		Description: Get the thread name
		Return: the thread name
	*/
	inline const char *GetName() { return m_szName; }
	
	/*
		Function: GetDatabaseStatus
		Description: Get the database status
		Return: the database status
	*/
	const char *GetDatabaseStatus();
	
	int GetExitCode();
	bool IsRunning();

private:
	char m_szName[MAX_MODULENAME+1];
	Module_EntryPoint m_cbMain;
	
	CDatabase* m_lpDatabase;
	CThreadServer* m_lpServer;
	ModuleConfigMap m_cfg;

#ifdef _WIN32
	HANDLE* m_lpDLL;
#else
	void* m_lpDLL;
#endif

};

#endif
