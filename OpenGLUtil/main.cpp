#include "stdafx.h"
#include "OpenGLUtil.hpp"
#include "ModelArray.h"

struct dummy{

};

int main()
{
	using namespace glutil;
	detail::MultiContainer<int, float, double, char> cont;
	cont.push_back(1, 10.f, 20.f, 'x');
	auto tup = cont[0];
	return 0;
}