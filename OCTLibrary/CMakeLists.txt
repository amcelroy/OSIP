cmake_minimum_required (VERSION 3.3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native")

project(OSIP)

set(WEBSOCKETPP_H "${CMAKE_CURRENT_SOURCE_DIR}/../dependancies" CACHE PATH "Websocketpp path")

set(ZLIB_H "/usr/include" CACHE PATH "Path to ZLib Header")
set(ZLIB_LIB "/usr/local/lib/libz.so" CACHE FILEPATH "Filepath to ZLib Library")

set(FFTWF_H "/usr/include" CACHE PATH "Path to FFTW Header")
set(FFTWF_LIB "/usr/lib64/libfftw3f.so" CACHE PATH "Filepath to fftwf library")

set(BOOST_H "" CACHE PATH "Boost path")
set(BOOST_LIB_DIR "" CACHE PATH "Path to boost libs")

set(ALAZAR_H "" CACHE PATH "Alzar Tech Header Path")
set(ALAZAR_LIB "" CACHE FILEPATH "Alazar libats")

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
					${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline
					${WEBSOCKETPP_H}
					${ZLIB_H}
					${FFTWF_H}
					${BOOST_H}
					${ALAZAR_H}
					)

add_executable(OSIP 
				main.cpp 
				websocketserver.cpp 
				octpipelinestage_cpu.cpp
				octconfigfile.cpp
				octdisplaystage.cpp
				)

target_link_libraries(OSIP -lz -lpthread -lfftw3f -lfftw3f_threads -lboost_system -lATSApi)