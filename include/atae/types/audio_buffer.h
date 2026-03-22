#pragma once
#include <vector>

struct AudioBuffer {
    std::vector<double> samples; // Interleaved if stereo
    int channels;
    int sampleRate;
};