#pragma once
#include "../../../../tools/audio_rw/audio_file.h"

struct Harmonic {
	double amplitude; // 1 / harmonic
	double phase_increment; // 2π * f * harmonic / sample_rate
	double phase; // current phase for this harmonic
};


class SawWave {
public:
	static AudioBuffer generate_naive(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude);
	static AudioBuffer generate(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude);
};