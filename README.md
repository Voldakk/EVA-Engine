# EVA Engine

A C++ OpenGL game engine

### Architecture/Framework
* Component based system where each 'game object' is only responsible for managing the components attached to it.  
Some component like the Transform and MeshRenderer receives special treatment, but the user is free to create any sort of component they want.
* InputManager for eays access to both mouse and keyboard input.
* The SceneManager provides an easy way to create multiple seperate scenes and change between them. Multiple scenes can be loaded at once.

### Aesthetics/Quality of Graphics
The engine uses the Phong lighting model. It supports: 
* Diffuse, specular, emission and normal maps.
* Multiple directional and point lights.
* Multiple directional shadow maps and a single cube map.
* A skybox.
* Instanced rendering.
* FreeType text.

## Setup

### Windows - Visual Studio 2017
1. File -> Open -> Folder - Select the root folder
2. Select "main" as the startup item

### Windows - CMake (cmake-gui)
1. Set the source code file to the root directory containing "CMakeLists.txt"
2. Set the build path to "\build\"
3. Click the configure button and select which generator you would like to use
4. Click the generate button
5. If your generator is an IDE such as Visual Studio, then open up the newly created EVA.sln file and set "main" as you StartUp Project.

### Ubuntu - CLion 2017
1. File -> Open - Select the root folder
2. Select the "main" configuration

### Ubuntu - Cmake terminal
Open a terminal window in the root folder of the project

##### 1. Create a build directory
```
mkdir build
cd build
```
##### 2. Generate UNIX Makefile (point CMake to CMakeLists.txt)
```
cmake ..
```
##### 3. Execute make command
```
make
```
##### 4. Run executable
```
cd bin
./main
```
## Test scenes

Thera are 3 test scenes in addition to a pacman game.
To run them simply uncomment the line in source/main.cpp

## Built With

* [GLEW](http://glew.sourceforge.net/)
* [GLFW](http://www.glfw.org/)
* [GLM](https://glm.g-truc.net/0.9.8/index.html)
* [FreeType](https://www.freetype.org/)
* [ASSIMP](http://assimp.org/)
* [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
* [RapidJSON](https://github.com/Tencent/rapidjson)

## Authors

* **Eivind Vold Aunebakk** - [Voldakk](https://github.com/Voldakk)

## Acknowledgments

* [TheChernoProject - Abstracting OpenGL into Classes](https://www.youtube.com/watch?v=bTHqmzjm2UI)
* [TheChernoProject - Buffer Layout Abstraction in OpenGL](https://www.youtube.com/watch?v=oD1dvfbyf6A)
* [TheChernoProject - Shader Abstraction in OpenGL](https://www.youtube.com/watch?v=gDtHL6hy9R8)
* [LearnOpenGL - Basic Lighting](https://learnopengl.com/Lighting/Basic-Lighting)
* [LearnOpenGL - Multiple lights](https://learnopengl.com/Lighting/Multiple-lights)
* [LearnOpenGL - Gamma Correction](https://learnopengl.com/Advanced-Lighting/Gamma-Correction)
* [LearnOpenGL - Shadow Mapping](https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping)
* [LearnOpenGL - Point Shadows](https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows)
* [LearnOpenGL - Normal Mapping](https://learnopengl.com/Advanced-Lighting/Normal-Mapping)
* [LearnOpenGL - Instancing](https://learnopengl.com/Advanced-OpenGL/Instancing)
* [LearnOpenGL - Text Rendering](https://learnopengl.com/In-Practice/Text-Rendering)
* [Stack Overflow - String to type mapping](https://stackoverflow.com/a/582456)