set (EXAMPLE_HDRS
		Application.h
		Perspective.h
		TrivialSceneManagerTest.h)

set (EXAMPLE_SRCS
		Main.cpp)

add_executable (example ${EXAMPLE_HDRS} ${EXAMPLE_SRCS})
target_link_libraries (example ${CORELIBS} glutapp)

add_executable (PngTest PngTest.cpp)
target_link_libraries (PngTest glutapp)

