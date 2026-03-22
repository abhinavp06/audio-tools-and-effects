#pragma once
#include "atae/types/audio_buffer.h"
#include "atae/types/harmonic.h"

class SawWave {
public:
	static AudioBuffer generate_naive(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude);
	static AudioBuffer generate(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude);
};