![alt-text](https://github.com/amcelroy/OSIP/blob/master/Cockrell_RGB_formal_BME.jpg)

# OSIP
Open Source Imaging Platform (OSIP)

Welcome to the University of Texas Biomedical Engineering Laser Lab OSIP project! 

## What is OSIP?

OSIP is a general purpose Open Source platform for acquiring, packaging, chaining processing, and saving steps. 
Our lab focuses on Optical Coherence Tomography, so there is a slight emphisis on that in the examples and master code. The 
code is written mostly in C++11 with front end work done in Qt. Qt is the main IDE used, as it is open source and cross platform.

## OSIP Dependancies

OSIP was built on the shoulders of giants. Here are some of the giants that OSIP uses and thanks for their contributions to the software field (alphabetical order).

 * Boost - Used for pathing and signals / slots, and just about anywhere else. Boost Filesystem library is required and must be compiled (macOS precompiled is provided)
 * C++ Committee
 * HDF5 - Used as the underpinning for saving N dimensional data. The HDF5 library is required (macOS is precompiled), see: https://support.hdfgroup.org/HDF5/release/cmakebuild.html
 * OpenCL - Cross platform GPU / CPU computing library, has its of Pipeline Stage (OpenCLPipeline)

## OSIP Concepts
Below outlines the following concepts of how OSIP interacts with itself and why it may help out your DAQ system. Through out the documentation, classes I represents the input class, and class O represents the output class.

### PipelineStage<class I, class O>
One of the main class that forms the underpinning of OSIP. The PipelineStage is a templated class where I and O can be any 
combination of the following:
  * char
  * unsigned char
  * short
  * unsigned short
  * int
  * unsigned int
  * float
  * double
  * long
  * unsigned long
  
PipelineStages are connected via an Inlet&lt;I> class, each PipelineStage has at least one Inlet&lt;I>, even if it isn't used, as in the case of a data acquistion stage. A PipelineStage may have many connecting Inlet&lt;O> classes from other PipelineStages. For example, a daq stage would flow into a saving stage and a processing stage.

### Inlet&lt;I> 

The Inlet&lt;I> class exists in all PipelineStages and consists of a thread safe data queue that lossily keeps track of input data.
Internally, there is an std::queue that holds Payload data (see below) and is protected by a mutex to prevent cross-thread 
read / write issues.

### Payload&lt;O>

A Payload class represents N dimensional data that is packaged and is dispatched and fetched from Inlets. Internally, the Payload
consists of a vector<vector<unsigned int>> of dimensions and a vector<shared_ptr&lt;I>> of I* data produced in the Pipeline. Payloads automatically wrap the I* pointer into a share_ptr&lt;I> so the user need not worry about it. A single Payload may be sent to multiple other PipelineStages thanks to the shared_ptr&lt;I>. 

For example, a user may produce a single 2D array of 4096 x 2048 data, in which case the Payload would contain:
  * Dimension Vector - vector<vector&lt;unsigned int>(4096, 2048)>>
  * Data - vector<shared_ptr&lt;I>(*data)>
Adding dimensions and data can be done using .addData(vector<unsigned int>, I*).

Once a Payload has been packaged, it should be considered READ ONLY!

Once the Payload&lt;I> has been used, it should be cleaned up by calling .finished() so the shared_ptr&lt;I> reference is decremented.



