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
		Function: LoadRetroSpyModule
		Description: Load a module and store it into the array
		Return: true if the module is loaded, otherwise false
		Parameters:
			name => The name of the module to be loaded
	*/
	bool LoadRetroSpyModule(const char *name);

	/*
		Function: LoadRetroSpyModule
		Description: Load a module and store it into the array
		Return: true if the module is loaded, otherwise false
		Parameters:
			name => The name of the module to be loaded
			cfg => A map to the configuration
	*/
	bool LoadRetroSpyModule(const char *name, ModuleConfigMap cfg);

	/*
		Function: Start
		Description: Start all the modules
	*/
	void Start();

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
