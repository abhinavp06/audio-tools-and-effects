#pragma once
#include <vector>
#include <random>
#include <cstdint>
#include <algorithm>
#include "atae/types/audio_buffer.h"

class PinkNoise {
public:
	static AudioBuffer generate(const int duration_s, const int sample_rate, const int channel_count, double amplitude);

private:
	static constexpr int NUM_ROWS = 16; // 16 is common, 20 is smoother
};