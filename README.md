# EVA Engine
A C++ OpenGL game engine

## PBR
![Image of pbr materials](screenshots/pbr.png?raw=true)

## Terrain
![Image of terrain](screenshots/terrain.png?raw=true)

## Water
![Image of water](screenshots/water.png?raw=true)


# Setup

### Windows - Visual Studio 2017
1. File -> Open -> Folder - Select the root folder
2. Make sure the Clang config is selected
3. Select "main" as the startup item

### Windows - CMake (cmake-gui)
1. Set the source code file to the root directory containing "CMakeLists.txt"
2. Set the build path to "\build\"
3. Click the configure button and select which generator you would like to use
4. Click the generate button
5. If your generator is an IDE such as Visual Studio, then open up the newly created EVA.sln file and set "main" as you StartUp Project.

### Ubuntu - CLion 2017
1. File -> Open - Select the root folder
2. Make sure the Clang config is selected
3. Select the "main" configuration

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

## Built With

* [GLEW](http://glew.sourceforge.net/)
* [GLFW](http://www.glfw.org/)
* [GLM](https://glm.g-truc.net/0.9.8/index.html)
* [FreeType](https://www.freetype.org/)
* [ASSIMP](http://assimp.org/)
* [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)
* [RapidJSON](https://github.com/Tencent/rapidjson)
* [ImGui](https://github.com/ocornut/imgui)
* [tiny file dialogs](https://sourceforge.net/projects/tinyfiledialogs/)

## Authors

* **Eivind Vold Aunebakk** - [Voldakk](https://github.com/Voldakk)
