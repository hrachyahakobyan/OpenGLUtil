// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOEXCEPT _NOEXCEPT

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>
#include <cmath>
#include <vector>
#include <memory>
#include <map>
#include <tuple>
#include <array>
#include <set>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

// SOIL

#include <SOIL.h>

#include "detail\standard_tuple.h"

#define Stringize( L )			#L
#define MakeString( M, L )		M(L)
#define $Line					\
	MakeString( Stringize, __LINE__ )
#define Reminder				\
	__FILE__ "(" $Line ") : Reminder: "

#define FORWARD_MODELARRAY() \
		template<typename, typename...> \
		class ModelArray;

#define FORWARD_MODELARRAYBASE() \
		template<typename> \
		class ModelArrayBase;

#define FRIEND_MODELARRAY() \
		template<typename, typename...> \
		friend class ModelArray; \
		template<typename> \
		friend class ModelArrayBase; 