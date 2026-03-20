#pragma once
#include "../../../tools/audio_rw/audio_file.h"

class WhiteNoise {
public:
	static AudioBuffer generate(const int duration_s, const int sample_rate, const int channel_count);
};