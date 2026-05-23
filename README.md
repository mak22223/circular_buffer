# Circular buffer library

This repository contains implementation of a circular buffer. 

The target usage platform is embedded devices. The implementation uses C++ templates, outputs and receives data via raw pointers and does not use dynamic memory.

Even though it is written on C++ it can be easily adapted for C. It is also possible to use this library (if you need to) on other platforms, such as devices operated by Linux, Windows and etc.

# Content

The library is a header-only library. It defines CircularBuffer template class with element type and size as template parameters.

Implementation contains the following features:
- put data in buffer
- get data from buffer (removes data from buffer)
- peek data from buffer (data remains in buffer)
- erase *N* elements from the head of the buffer
- find index of the first matching element in buffer
- get used storage size
- get available storage size
- check if buffer is empty
- check if buffer is full

The implementation has its funtionality tested via unit tests. The implementation is not tested for thread safety.

# License

This project is licensed under the MIT License.
