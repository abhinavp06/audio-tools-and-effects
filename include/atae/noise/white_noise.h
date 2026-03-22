#pragma once
#include "atae/types/audio_buffer.h"

class WhiteNoise {
public:
	static AudioBuffer generate(const int duration_s, const int sample_rate, const int channel_count);
};