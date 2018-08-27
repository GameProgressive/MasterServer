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
#ifndef _MASTERSERVER_MODULE_H
#define _MASTERSERVER_MODULE_H

#include "Thread.h"

#include <MDK/ModuleEntryPoint.h>

#include <string>
#include <map>

// MAX_PATH in Windows
#define MAX_MODULENAME 255

// Callbacks and typedef
typedef int (* Module_EntryPoint)(void*);
typedef bool (* Module_ConfigFunction)(ModuleConfigMap);

/*
	This class rappresents a dynamic module that will
	be loaded into RetroSpy Main Server
*/
class CModule : public CThread
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
	*/
	void Start();

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

private:
	char m_szName[MAX_MODULENAME+1];

	Module_EntryPoint m_cbMain;
	Module_ConfigFunction m_cbConfig;

	ModuleMain m_module;

	CDatabase* m_database;

	void UpdateThreadInformation();
};

#endif
