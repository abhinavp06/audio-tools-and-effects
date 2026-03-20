#!/bin/bash
echo "compiling necessary files for white noise generation...."
if ! g++ -std=c++20 -Wall -Wextra white_noise_test.cpp white_noise.cpp ../../../tools/audio_rw/audio_file.cpp -o white_noise.exe; then 
echo "compilation failed"
exit 1
fi
echo "done"