project (program1)
cmake_minimum_required(VERSION 3.15)

# configure OpenCV
find_package(OpenCV REQUIRED)

# create create individual projects
set(SOURCES main.cpp)
add_executable(main ${SOURCES})
target_link_libraries(main ${OpenCV_LIBS})
