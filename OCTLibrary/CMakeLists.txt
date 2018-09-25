cmake_minimum_required (VERSION 3.3)
project(OSIP)

set(WEBSOCKETPP_H "${CMAKE_CURRENT_SOURCE_DIR}/../dependancies" CACHE PATH "Websocketpp path")

set(ZLIB_H "/usr/include" CACHE PATH "Path to ZLib Header")
set(ZLIB_LIB "/usr/local/lib/libz.so" CACHE FILEPATH "Filepath to ZLib Library")

set(FFTWF_H "/usr/include" CACHE PATH "Path to FFTW Header")
set(FFTWF_LIB "/usr/lib64/libfftw3f.so" CACHE PATH "Filepath to fftwf library")

set(BOOST_H "" CACHE PATH "Boost path")
set(BOOST_LIB_DIR "" CACHE PATH "Path to boost libs")

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
					${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline
					${WEBSOCKETPP_H}
					${ZLIB_H}
					${FFTWF_H}
					${BOOST_H}
					)

add_executable(OSIP 
				main.cpp 
				websocketserver.cpp 
				octpipelinestage_cpu.cpp
				octdisplaystage.cpp
				octconfigfile.cpp
				)

target_link_libraries(OSIP -lz -lpthread -lboost_system)