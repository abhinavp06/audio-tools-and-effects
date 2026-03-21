#!/bin/bash
echo "compiling necessary files for saw wave generation...."
if ! g++ -std=c++20 -Wall -Wextra saw_wave_test.cpp saw_wave.cpp ../../../../tools/audio_rw/audio_file.cpp -o saw_wave.exe; then 
echo "compilation failed"
exit 1
fi
echo "done"