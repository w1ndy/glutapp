include_directories("../..")

add_executable (Perspective Main.cpp)
target_link_libraries (Perspective ${CORELIBS} glutapp)
