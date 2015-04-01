include_directories("../..")

add_executable (ObjectLoader Main.cpp)
target_link_libraries (ObjectLoader ${CORELIBS} glutapp)
