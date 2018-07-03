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
#include "ModuleManager.h"

CModuleManager::CModuleManager()
{
}

CModuleManager::~CModuleManager()
{
	size_t i = 0;

	// Free memory of the modules
	for (; i < m_vModules.size(); i++)
		delete m_vModules.at(i);

	m_vModules.clear();
}

bool CModuleManager::LoadMSModule(const char *name)
{
	CModule *module = NULL;

	// Add the pointer into the vector
	m_vModules.push_back(new CModule());

	// Get the module
	module = m_vModules.at(m_vModules.size() - 1);

	// Try to load the module
	if (!module->Load(name))
	{
		delete module;
		// Remove the module from the array
		m_vModules.erase(m_vModules.begin()+(m_vModules.size()-1));
		return false;
	}

	return true;
}


bool CModuleManager::LoadMSModule(const char *name, ModuleConfigMap cfg)
{
	CModule *module = NULL;

	// Add the pointer into the vector
	m_vModules.push_back(new CModule());

	// Get the module
	module = m_vModules.at(m_vModules.size() - 1);

	// Try to load the module
	if (!module->Load(name, cfg))
	{
		delete module;
		// Remove the module from the array
		m_vModules.erase(m_vModules.begin()+(m_vModules.size()-1));
		return false;
	}

	return true;
}

void CModuleManager::Start()
{
	size_t i = 0;

	for (; i < m_vModules.size(); i++)
	{
		LOG_INFO("Module", "Starting %s...", m_vModules.at(i)->GetName());
		m_vModules.at(i)->Start();
	}
}

void CModuleManager::Stop()
{
	size_t i = 0;

	for (; i < m_vModules.size(); i++)
	{
		m_vModules.at(i)->Stop();
	}
}

size_t CModuleManager::GetModuleSize()
{
	return m_vModules.size();
}

CModule* CModuleManager::GetModule(size_t index)
{
	if (GetModuleSize() < index)
		return NULL;

	return m_vModules.at(index);
}
