add_subdirectory(test)

add_library(Manager Manager.cpp)

add_executable(VDSProject main.cpp)
target_link_libraries(VDSProject Manager)
