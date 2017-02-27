# OpenGLUtil

OpenGLUtil is a collection of wrapper classes to ease the development of simple OpenGL applications. I started its development
in parallel with the assignments from my computer graphics course and I have used it extensively myself.
The library contains classes that abstract the boilerplate code of 

1. creating and configuring the OpenGL environment
2. windows
3. user input
4. camera
5. shader compilation and linkage
6. model loading
7. etc

The goal of the project is not to provide complete solutions to the users, but a template to build on.

I have extensively used the wonderful code and tutorials found on (Learn OpenGL)[https://learnopengl.com/] both for creation
of my own code and for learning purposes.

## Getting Started

To use OpenGLUtil you can either directly integrate the source code in your project or link your project with the static library
which can be found in the _lib/_ folder. Currently, OpenGLUtil only supports Visual Studio 2013, however it will also run on other versions
of Visual Studio if the libraries it uses are supported by the compiler.

### Prerequisites

OpenGLUtil utilizes the following libraries and frameworks
1. [glfw](http://www.glfw.org/)
2. [glew](http://glew.sourceforge.net/)
3. [soil](http://www.lonesock.net/soil.html)
4. [assimp](http://www.assimp.org/)

```
Examples coming soon!
```

## Acknowledgments

* [Learn OpenGL](https://learnopengl.com/) for wonderful tutorials, sample code and inspiration
