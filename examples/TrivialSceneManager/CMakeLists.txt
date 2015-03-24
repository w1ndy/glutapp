include_directories("../..")

add_executable (TrivialSceneManager Main.cpp)
target_link_libraries (TrivialSceneManager ${CORELIBS} glutapp)
