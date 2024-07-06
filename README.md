#!/bin/bash
git submodule update --init --recursive
cmake . -B build -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug

#Windows:
cmake . -B build -DLIBXML2_LIBRARY="C:\Program Files\libxml2\lib\libxml2s.lib" -DLIBXML2_INCLUDE_DIR="C:\Program Files\libxml2\include\libxml2"
