cmake_minimum_required (VERSION 2.8)
project (glutapp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set (CORELIBS
		${OPENGL_LIBRARIES}
		${GLUT_glut_LIBRARY})

include_directories(
		${PROJECT_SOURCE_DIR}
		${OPENGL_INCLUDE_DIRS}
		${GLUT_INCLUDE_DIRS})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (HDRS
		BaseType.h
		GlutApp.h
		GlutCamera.h
		GlutListener.h
		GlutImage.h
		GlutPNG.h
		GlutBounds2D.h
		GlutGraphics2D.h
		GlutSceneManager2D.h)

set (SRCS
		BaseType.cpp
		GlutApp.cpp
		GlutCamera.cpp
		GlutListener.cpp
		GlutImage.cpp
		GlutPNG.cpp
		GlutGraphics2D.cpp
		GlutSceneManager2D.cpp)

if (APPLE)
	set (CMAKE_CXX_FLAGS		"-Wall -std=c++11 -stdlib=libc++")
	set (CMAKE_CXX_FLAGS_DEBUG	"-g")
else ()
	set (CMAKE_CXX_FLAGS		"-Wall -std=c++11")
	set (CMAKE_CXX_FLAGS_DEBUG	"-g")
endif ()

set (CMAKE_C_FLAGS			"-std=c99 -Wall")
set (CMAKE_C_FLAGS_DEBUG	"-g")

add_library (glutapp STATIC ${HDRS} ${SRCS})
target_link_libraries (glutapp ${CORELIBS})
add_subdirectory (examples)

