project (program1)
cmake_minimum_required(VERSION 3.15)

find_package(OpenCV REQUIRED)

set(SOURCES program1.cpp)
add_executable(program1 ${SOURCES})
target_link_libraries(program1 ${OpenCV_LIBS})