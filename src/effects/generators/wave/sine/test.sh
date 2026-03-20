#!/bin/bash
echo "compiling necessary files for white noise generation...."
if ! g++ -std=c++20 -Wall -Wextra sine_wave_test.cpp sine_wave.cpp ../../../../tools/audio_rw/audio_file.cpp -o sine_wave.exe; then 
echo "compilation failed"
exit 1
fi
echo "done"