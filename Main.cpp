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
#include "MSConfig.h"

#include <MDK/Utility.h>

#include <iostream>
#include <string>

#include <stdlib.h>
#include <time.h>

#if defined(_WIN32) && defined(_DEBUG) && defined(HAVE_VLD_H)
#include <vld.h>
#endif

#define CONFIG_NAME "MasterServer.ini"
#define MASTERSERVER_VERSION "Dev1"

// Help defines
#define MAX_CMDSTR 100
#define MAX_CMDDSC 300

typedef struct THelp
{
	char name[MAX_CMDSTR];
	char desc[MAX_CMDDSC];
} SHelp;

void ConsolePause();

// Commands
void _PrintHelp();
void _PrintModuleInfo(CModuleManager* mngr);

bool Main_Running = true;

// Help Command
const SHelp HelpArray[] = {
	{ "quit",				"Close the Server and exit" },
	{ "help",				"Print this information screen" },
	{ "moduleinfo",			"Print the information about the modules" }
};
const int HelpArray_Len = 3;

/*
	Function: Main
	Description: Entry Point of the Server
*/
int main()
{
	// Create the main instances
	CModuleManager mm;
	//CDatabase db;
	
	// We use this variable for console input
	// I defined this here, so we won't create
	// a variable each call of the loop
	std::string consoleInput = "";

	srand((unsigned int)time(NULL));

	printf("MasterServer ~The universal Server container~ v.%s\n", MASTERSERVER_VERSION);
	
	// Load the configuration
	if (!CConfig::Load(&mm, CONFIG_NAME))
	{
		LOG_ERROR("MasterServer", "Cannot load %s\n", CONFIG_NAME);
		ConsolePause();
		return 0;
	}

	// Start our modules
	mm.Start();

	LOG_INFO("MasterServer", "Server Started!");
	LOG_INFO("MasterServer", "Type \"help\" for the list of commands avaiable.");

	// Application loop
	while (Main_Running)
	{
		consoleInput.clear();

		printf(">");

		// Read the input and process that
		std::cin >> consoleInput;

		if (strcmp(consoleInput.c_str(), "quit") == 0)
			Main_Running = false;
		else if (strcmp(consoleInput.c_str(), "help") == 0)
			_PrintHelp();
		else if (strcmp(consoleInput.c_str(), "moduleinfo") == 0)
			_PrintModuleInfo(&mm);
		else
			LOG_WARN("MasterServer", "Unknown command \"%s\"", consoleInput.c_str());

	}

	// Stop the modules
	mm.Stop();

	ConsolePause();
	return 0;
}

/*
	Function: _PrintHelp
	Description: Prints the command list
*/
void _PrintHelp()
{
	int i = 0;

	LOG_INFO("MasterServer", "List of Avaiable Commands:");
	
	for (; i < HelpArray_Len; i++)
	{
		LOG_INFO("MasterServer", "%s   %s", HelpArray[i].name, HelpArray[i].desc);
	}
}

const char* ExitCodeToString(unsigned long ecode)
{
	switch (ecode)
	{
	case ERROR_NONE:
		return "No Error";
		break;
	case ERROR_MYSQL_POINTER:
		return "Invalid MySQL Connection";
		break;
	case ERROR_BIND_ERROR:
		return "Unable to Bind";
		break;
	case ERROR_STILL_ALIVE:
		return "Running Process";
		break;
	case ERROR_UNKNOWN:
	default:
		return "Unknown Error";
		break;
	}

	return "";
}

/*
	Function: _PrintModuleInfo
	Description: Print the status of the modules
*/
void _PrintModuleInfo(CModuleManager* mngr)
{
	LOG_INFO("MasterServer", "Loaded modules: %u", mngr->GetModuleSize());

	if (mngr->GetModuleSize() > 0)
	{
		size_t i = 0;
		for (; i < mngr->GetModuleSize(); i++)
		{
			CModule *m = mngr->GetModule(i);
			LOG_INFO("MasterServer", "Module %u: %s   Status: %s (Code: %s)   Database Status: %s", i, m->GetName(), m->IsRunning() ? "Running" : "Stopped", ExitCodeToString(m->GetExitCode()), m->GetDatabaseStatus());
		}
	}
}

/*
	Function: ConsolePause
	Descrption: Simulate "pause" command of Windows
*/
void ConsolePause()
{
	char k = 0;

	printf("Press ENTER to exit...");

#if _WIN32 && _MSC_VER >= 1400
	scanf_s("%1c", &k, 1);
#else
	scanf("%1c", &k);
#endif

	getchar();

}
