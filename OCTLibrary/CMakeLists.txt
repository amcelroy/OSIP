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
set(FFTWF_LIB "/usr/lib64/libfftw3f.so" CACHE FILEPATH "Filepath to fftwf library")

set(HDF5_H "" CACHE PATH "Path to HDF5 Headers")
set(HDF5_H_LIB "" CACHE FILEPATH "Filepath to HDF5 library")

set(BOOST_H "" CACHE PATH "Boost path")
set(BOOST_LIB_DIR "" CACHE PATH "Path to boost libs")

set(ALAZAR_H "" CACHE PATH "Alzar Tech Header Path")
set(ALAZAR_LIB "" CACHE FILEPATH "Alazar libats")

set(LIBPNG_H "" CACHE PATH "Path to LIBPNG Headers")
set(LIBPNG_LIB "" CACHE FILEPATH "Filepath to LIBPNG library")

set(NIDAQmx_LIB "" CACHE FILEPATH "Path to NIDAQmx Library")
set(NIDAQmx_H "" CACHE PATH "Path to NIDAQmx Headers")

find_package(Threads)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
					${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline
					${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/Peripherals
					${CMAKE_CURRENT_SOURCE_DIR}/../Peripheral
					${WEBSOCKETPP_H}
					${ZLIB_H}
					${FFTWF_H}
					${BOOST_H}
					${ALAZAR_H}
					${HDF5_H}
					${LIBPNG_H}
					${NIDAQmx_H}
					)

link_directories(${BOOST_LIB_DIR})

add_executable(OSIP 
				main.cpp 
				websocketserver.cpp 
				websocketserver.h
				octpipelinestage_cpu.cpp
				octpipelinestage_cpu.h
				octconfigfile.cpp
				octconfigfile.h
				octdisplaystage.cpp
				octdisplaystage.h
				loadoctpipeline.hpp 
				octpipeline.h
				${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/DAQ/Alazar/daqstagealazar.hpp
				${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/DAQ/Alazar/daqstagealazar660.hpp
				${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/DAQ/Alazar/daqstagealazar9350.hpp
				${CMAKE_CURRENT_SOURCE_DIR}/../Peripheral/nidaqmxgalvos.cpp
				)

target_link_libraries(OSIP ${ZLIB_LIB} ${FFTWF_LIB} ${ALAZAR_LIB} ${LIBPNG_LIB} ${NIDAQmx_LIB})