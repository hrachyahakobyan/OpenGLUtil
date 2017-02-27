// Samples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModelSampleApp.h"


int _tmain(int argc, _TCHAR* argv[])
{
	ModelSampleApp app("Nanosuit");
	app.initialize();
	app.run();
	return 0;
}

