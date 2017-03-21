// Samples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModelSampleApp.h"
#include "InstanceSampleApp.h"


static void modelSampleApp()
{
	ModelSampleApp app("Nanosuit");
	app.initialize();
	app.run();
}

static void instanceSampleApp()
{
	InstanceSampleApp app("Asteroid");
	app.initialize();
	app.run();
}

int _tmain(int argc, _TCHAR* argv[])
{
	modelSampleApp();
	return 0;
}

