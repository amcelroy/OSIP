cmake_minimum_required (VERSION 3.3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native")
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

project(OSIP)

set(WEBSOCKETPP_H "${CMAKE_CURRENT_SOURCE_DIR}/../dependancies" CACHE PATH "Websocketpp path")

set(ZLIB_H "/usr/include" CACHE PATH "Path to ZLib Header")
set(ZLIB_LIB "/usr/local/lib/libz.so" CACHE FILEPATH "Filepath to ZLib Library")

set(FFTWF_H "/usr/include" CACHE PATH "Path to FFTW Header")
set(FFTWF_LIB "/usr/lib64/libfftw3f.so" CACHE FILEPATH "Filepath to fftwf library")

set(HDF5_H "" CACHE PATH "Path to HDF5 Headers")
set(HDF5_LIB_DIR "" CACHE PATH "Filepath to HDF5 libraries")
set(HDF5_C_LIB "libhdf5.lib")
set(HDF5_C_D_LIB "libhdf5_D.lib")
set(HDF5_CPP_LIB "libhdf5_cpp.lib")
set(HDF5_CPP_D_LIB "libhdf5_cpp_D.lib")
set(HDF5_CPP_HL_LIB "libhdf5_hl_cpp.lib")
set(HDF5_CPP_D_HL_LIB "libhdf5_hl_cpp_D.lib")

set(ALAZAR_H "" CACHE PATH "Alzar Tech Header Path")
set(ALAZAR_LIB "" CACHE FILEPATH "Alazar libats")

set(LIBPNG_H "" CACHE PATH "Path to LIBPNG Headers")
set(LIBPNG_LIB "" CACHE FILEPATH "Filepath to LIBPNG library")

set(NIDAQmx_LIB "" CACHE FILEPATH "Path to NIDAQmx Library")
set(NIDAQmx_H "" CACHE PATH "Path to NIDAQmx Headers")

set(SZIP_LIB "" CACHE FILEPATH "Path to SZIP Library")
set(SZIP_H "" CACHE PATH "Path to SZIP Headers")

set(HTTPLIB_H "" CACHE PATH "Path to HTTPLIB Headers")

set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Threads)
find_package(Boost 1.68.0 COMPONENTS filesystem)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
					${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline
					${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/Peripherals
					${CMAKE_CURRENT_SOURCE_DIR}/../Peripheral
					${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/DAQ
					${WEBSOCKETPP_H}
					${ZLIB_H}
					${FFTWF_H}
					${Boost_INCLUDE_DIRS}
					${ALAZAR_H}
					${HDF5_H}
					${LIBPNG_H}
					${NIDAQmx_H}
					${HTTPLIB_H}
					)

link_directories(${Boost_LIBRARY_DIRS} ${HDF5_LIB_DIR})

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
				${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/DAQ/daqstagegenerated.hpp
				${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/DAQ/Alazar/daqstagealazar.hpp
				${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/DAQ/Alazar/daqstagealazar660.hpp
				${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/DAQ/Alazar/daqstagealazar9350.hpp
				${CMAKE_CURRENT_SOURCE_DIR}/../Peripheral/nidaqmxgalvos.cpp
				)

add_executable(OSIPSaving 
				main_saving.cpp 
				octconfigfile.cpp
				octconfigfile.h
				octpipelinestage_cpu.cpp
				octpipelinestage_cpu.h
				${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/DAQ/daqstagegenerated.hpp
				${CMAKE_CURRENT_SOURCE_DIR}/../Pipeline/savingstage.hpp
				)

target_link_libraries(OSIP ${ZLIB_LIB} ${FFTWF_LIB} ${ALAZAR_LIB} ${LIBPNG_LIB} ${NIDAQmx_LIB} ${Boost_LIBRARIES})
target_link_libraries(OSIPSaving 
						optimized ${ZLIB_LIB} 
						${Boost_LIBRARIES} 
						optimized ${HDF5_C_LIB} 
						optimized ${HDF5_CPP_LIB} 
						optimized ${HDF5_CPP_HL_LIB} 
						${SZIP_LIB}
						${ZLIB_LIB} 
						${FFTWF_LIB}
						debug ${HDF5_C_D_LIB} 
						debug ${HDF5_CPP_D_LIB} 
						debug ${HDF5_CPP_D_HL_LIB})

