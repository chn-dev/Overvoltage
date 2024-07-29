Overvoltage
===========

This is a software sampler plugin, written using JUCE in C++. The old freeware plugin shortcircuit2 (or sc2) has been the big role model because of its ease of use and creativity-inducing audio-sculpting functions. Unfortunately, sc2 hasn't been updated for quite a long time and is only available as a 32bit VST2 plugin. This and the fact that I always wanted to develop my own instrument plugin are the reasons why I started Overvoltage.

![Overvoltage Screenshot](Overvoltage-20240729.png)

The current state of Overvoltage should be considered highly experimental, unstable and hence far from ready to be used in a productive manner. But it hopefully will be one day.

## Cloning from git

    git clone https://github.com/chn-dev/Overvoltage
    cd Overvoltage
    git submodule update --init --recursive

The last step is necessary because the JUCE repository is included as a submodule in ./Overvoltage/JUCE/. The last git command above will automatically clone the right version of JUCE to that directory.

## Compiling

### Linux

    cmake . -B build
    cmake --build build

### Windows

    cmake . -B build -DLIBXML2_LIBRARY="C:\Program Files\libxml2\lib\libxml2s.lib" -DLIBXML2_INCLUDE_DIR="C:\Program Files\libxml2\include\libxml2"
    cmake --build build

Overvoltage depends on libxml2 for loading/saving its data, so libxml2 needs to be compiled and installed under Windows as well. The variables LIBXML2_LIBRARY and LIBXML2_INCLUDE_DIR tell cmake where to find the library file and the header files of libxml2.
