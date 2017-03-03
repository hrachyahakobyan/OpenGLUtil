#include "stdafx.h"
#include "OpenGLUtil.hpp"
#include "InstanceSampleApp.h"

int main()
{
	using namespace glutil;
	InstanceSampleApp app("Hi");
	app.initialize();
	app.run();
	return 0;
}