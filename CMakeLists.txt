cmake_minimum_required(VERSION 3.1)

add_definitions(-DFOO)

# Project name
project(EVA-Engine)

# System type
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	SET( LINUX 1)
	SET( APPLE 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET( LINUX 0)
	SET( APPLE 1 ) 
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	SET( WINDOWS 1)
endif()

# Set GLFW options
set(GLFW_BUILD_EXAMPLES OFF) 
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

# Find packages
find_package(OpenGL REQUIRED)

if(APPLE OR LINUX)
	find_package(GLEW REQUIRED)
endif(APPLE OR LINUX)

if(WINDOWS)
	link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLEW/lib/)
	link_directories(${PROJECT_SOURCE_DIR}/Dependencies/GLFW/lib-vc2015/)
	link_directories(${PROJECT_SOURCE_DIR}/Dependencies/freetype/lib/)
	link_directories(${PROJECT_SOURCE_DIR}/Dependencies/assimp/lib/)
endif(WINDOWS)

# Main executeable
file(GLOB SOURCES
	EVA-Engine/source/*
	EVA-Engine/source/*/*
	EVA-Engine/EVA/*
	EVA-Engine/EVA/*/*
	EVA-Engine/vendor/*/*
)

add_executable(main ${SOURCES})

set_target_properties(
    main
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Include and link
target_include_directories(
    main 
	PRIVATE ${PROJECT_SOURCE_DIR}/EVA-Engine/EVA/include
	
	PRIVATE ${PROJECT_SOURCE_DIR}/Dependencies/GLEW/include
	PRIVATE ${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include
	PRIVATE ${PROJECT_SOURCE_DIR}/Dependencies/glm/include
	PRIVATE ${PROJECT_SOURCE_DIR}/Dependencies/assimp/include/
	PRIVATE ${PROJECT_SOURCE_DIR}/Dependencies/freetype/include/
	PRIVATE ${PROJECT_SOURCE_DIR}/Dependencies/rapidjson/include/
	
	PRIVATE ${PROJECT_SOURCE_DIR}/EVA-Engine/vendor/stb-image
	PRIVATE ${PROJECT_SOURCE_DIR}/EVA-Engine/vendor/imgui
	PRIVATE ${PROJECT_SOURCE_DIR}/EVA-Engine/vendor/tfd
)

if(WINDOWS)
	target_link_libraries(
		main 
		glew32
		glfw3
		opengl32
		assimp-vc140-mt
		freetype
	) 

	file(
			COPY  ${PROJECT_SOURCE_DIR}/Dependencies/GLEW/lib/glew32.dll
			DESTINATION ${PROJECT_BINARY_DIR}/bin
	)
	file(
			COPY  ${PROJECT_SOURCE_DIR}/Dependencies/assimp/lib/assimp-vc140-mt.dll
			DESTINATION ${PROJECT_BINARY_DIR}/bin
	)
else()
	target_link_libraries(
		main 
		${GLEW_LIBRARY}
	)
endif(WINDOWS)

if(LINUX)
	add_library(stdc++fs UNKNOWN IMPORTED)
	set_property(TARGET stdc++fs PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/Dependencies/stdfs/libstdc++fs.a")
	target_link_libraries(
			main
			stdc++fs
	)
endif(LINUX)

# Apple
if(APPLE)
	set(
		CMAKE_CXX_FLAGS 
		"${CMAKE_CXX_FLAGS} -I${GLEW_INCLUDE_DIRS}"
	)
endif(APPLE)


# Copy assets
file(COPY  ${PROJECT_SOURCE_DIR}/EVA-Engine/assets DESTINATION ${PROJECT_BINARY_DIR}/bin)

find_package(Doxygen)
#Set up doxygen if it is installed
if(DOXYGEN_FOUND)

	message("Setting up doxygen")
	set(DOXYGEN_IN ${PROJECT_BINARY_DIR}/CMakeDoxyfile.in)
	set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/Doxyfile)

	# Setting folders to generate documentation for
	set(DOXYGEN_INPUT 
		"${PROJECT_SOURCE_DIR}/EVA-Engine/source ${PROJECT_SOURCE_DIR}/EVA-Engine/EVA"
	)
	set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/doc)
	set(DOXYGEN_INCLUDE_GRAPH YES)

	# This first one is optional
	set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
	set(DOXYGEN_EXTRACT_ALL YES)
	set(DOXYGEN_EXTRACT_STATIC YES)

	set(DOXYGEN_EXTRACT_STATIC YES)
	set(DOXYGEN_HAVE_DOT YES)

	set(DOXYGEN_DOT_NUM_THREADS 4)
	set(DOXYGEN_CALL_GRAPH  YES)
	set(DOXYGEN_CALLER_GRAPH YES)
	set(DOXYGEN_RECURSIVE YES)

	#DOT_PATH = ${DOXYGEN_DOT_PATH}

	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

	add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT} 
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )

	#set_target_properties(doc_doxygen
	#	PROPERTIES
	#	DOXYGEN_INPUT ""
	#	DOXYGEN_OUTPUT_DIRECTORY
	#)
else()

	message("Could not find doxygen")

endif(DOXYGEN_FOUND)
