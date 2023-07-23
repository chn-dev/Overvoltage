#!/bin/bash
git submodule update --init --recursive
cmake . -B build -G "CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
