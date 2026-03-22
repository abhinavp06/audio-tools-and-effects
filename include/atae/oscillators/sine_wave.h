#pragma once
#include "atae/types/audio_buffer.h"
#include "atae/oscillators/oscillator.h"

class SineWave : public Oscillator {
public:
	AudioBuffer generate_naive(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) override;
	AudioBuffer generate(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) override;
};