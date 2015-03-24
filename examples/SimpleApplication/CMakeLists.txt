include_directories("../..")

add_executable (SimpleApplication Main.cpp)
target_link_libraries (SimpleApplication ${CORELIBS} glutapp)
