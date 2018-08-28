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
#ifndef _MASTERSERVER_MODULEMANAGER_H
#define _MASTERSERVER_MODULEMANAGER_H

#include "Module.h"

#include <vector>

/*
	This class manage all the modules that will be loaded
	with the Server
*/
class CModuleManager
{
public:
	CModuleManager();
	~CModuleManager();

	/*
		Function: LoadMSModule
		Description: Load a module and store it into the array
		Return: true if the module is loaded, otherwise false
		Parameters:
			name => The name of the module to be loaded
	*/
	bool LoadMSModule(const char *name);

	/*
		Function: LoadMSModule
		Description: Load a module and store it into the array
		Return: true if the module is loaded, otherwise false
		Parameters:
			name => The name of the module to be loaded
			cfg => A map to the configuration
	*/
	bool LoadMSModule(const char *name, ModuleConfigMap cfg);

	/*
		Function: Start
		Description: Start all the modules
	*/
	void Start(CDatabase* db);

	/*
		Function: Stop
		Description: Stop all the modules
	*/
	void Stop();

	/*
		Function: GetModuleSize
		Description: Get the count of the modules
	*/
	size_t GetModuleSize();

	/*
		Function: GetModule
		Description: Get a module
		Return: A pointer to the module or NULL if the module dosen't exists at the specified index
		Parametres:
			index => The number of the module to get
	*/
	CModule *GetModule(size_t index);

protected:
	std::vector<CModule*> m_vModules;
};

#endif
