// Samples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ModelSampleApp.h"

template<typename

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << glm::mat4::length() << std::endl;
	std::cout << glm::vec4::length() << std::endl;

	ModelSampleApp app("Nanosuit");
	app.initialize();
	app.run();
	return 0;
}

