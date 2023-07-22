cmake_minimum_required(VERSION 3.0.0)
project(ThreadPool VERSION 0.1.0 LANGUAGES C CXX)

# include(CTest)
# enable_testing()

set (SRC_LST
	main.cpp
	threadpool.h
	threadpool.cpp
	workthread.h
)

add_executable(ThreadPool ${SRC_LST})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
