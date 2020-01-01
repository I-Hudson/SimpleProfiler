#include <iostream>

#include "ProfileApp.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main()
{
	Profile::Instrumentor* session = Profile::Instrumentor::Get();
	session->BeginSession("App");
	
	ProfileApp* app = new ProfileApp();
	app->Run();
	delete app;
	
	session->EndSession();
	session->ResetProfiler();

	//Profile::ProfilerDataProcessor data;
	//data.Load("json.json");

	_CrtDumpMemoryLeaks();

	return 0;
}