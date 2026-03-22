#pragma once
#include "atae/types/audio_buffer.h"

class Oscillator {
public:
	virtual AudioBuffer generate_naive(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) = 0;
	virtual AudioBuffer generate(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) = 0;
	virtual ~Oscillator() = default;
protected:
	void isValidRequest(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude);
	AudioBuffer setupAudioBuffer(const int duration_s, const int sample_rate, const int channel_count);
};