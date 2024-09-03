Overvoltage
===========

This is a software sampler plugin, written using JUCE in C++. The old freeware plugin shortcircuit2 (or sc2) has been the big role model because of its ease of use and creativity-inducing audio-sculpting functions. Unfortunately, sc2 hasn't been updated for quite a long time and is only available as a 32bit VST2 plugin. This and the fact that I always wanted to develop my own instrument plugin are the reasons why I started Overvoltage.

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

Overvoltage depends on libxml2 for loading/saving its data, so libxml2 needs to be compiled and installed under Windows before compiling Overvoltage. The variables LIBXML2_LIBRARY and LIBXML2_INCLUDE_DIR tell cmake where to find the library file and the header files of libxml2.

Depending on libxml2 rather than the internal XML framework of JUCE is a design decision to achieve a strict separation between the sampler's GUI (src/SamplerGUI), the sampler engine (src/SamplerEngine) and the interfacing with the plugin's host using the LV2, VST3 or any other interface specification (JUCE). This way, the SamplerEngine can be used in isolation to just play music in a game, for example. The dependency on JUCE can be omitted and only the dependency on libxml2 remains.

## Overview

![Overvoltage Screenshot](Overvoltage-20240729.png)

An instance of Overvoltage consists of a program containing 16 parts (which correspond to MIDI channels 1 to 16). Each part consists of 8 layers A..H, each containing an arbitrary number of samples. You can drag&drop WAV files from your operating system's file explorer to Overvoltage's sample area located right beside the keybard area. Each block in the sample area corresponds to a sample. You can activate sample(s) by clicking on them or by marking them with a selection rectangle. 