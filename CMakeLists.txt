cmake_minimum_required(VERSION 3.0.0)
project(test_project VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 17)

include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${PROJECT_SOURCE_DIR}/lib")

add_executable(test_project main.c)
TARGET_LINK_LIBRARIES( test_project PUBLIC raylib opengl32 gdi32 winmm)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
