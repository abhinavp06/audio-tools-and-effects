#!/bin/bash
echo "compiling necessary files for square wave generation...."
if ! g++ -std=c++20 -Wall -Wextra square_wave_test.cpp square_wave.cpp ../../../../tools/audio_rw/audio_file.cpp -o square_wave.exe; then 
echo "compilation failed"
exit 1
fi
echo "done"