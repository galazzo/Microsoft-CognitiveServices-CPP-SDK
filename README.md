# Microsoft Cognitive Services C++ SDK

This repo is not an official SDK. The project is a wrapper to use Microsoft Cognitive Services in standard C++ as is currently not supported.

The project provide full support to Computer Vision API. A wrapper to **gphoto2** will provide the full control digital cameras (**DSLR**), getting the raw shot and making manipulations with Computer Vision API.

### Compile examples

***
Because of the support to **gphoto2** you need some packages are required. In Ubuntu you can install:

* apt-get install build-essential
* apt-get install libcurl4-openssl-dev (libcurl)
* apt-get install libjsoncpp-dev (libjson)
* apt-get install pkg-config
* apt-get install gphoto2
* apt-get install libgphoto2-dev

Into path _examples/_ 
`cmake .`

if all packages are found you can run `make -j 4`

one compiled all examples you can test:

`bin/recognize-domain-specific-content -model landmarks -i images/sample06.jpg`
