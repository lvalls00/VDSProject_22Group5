add_subdirectory(test)

add_subdirectory(bench)
add_subdirectory(verify)
add_subdirectory(reachability)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror ")
add_library(Manager Manager.cpp)

add_executable(VDSProject main.cpp)
target_link_libraries(VDSProject Manager)
