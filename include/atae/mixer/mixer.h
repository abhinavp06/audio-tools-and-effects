#pragma once
#include <vector>
#include "atae/types/audio_buffer.h"

struct MixInput {
	AudioBuffer audio_buffer;
	double gain;
};

class Mixer {
public:
	virtual AudioBuffer mix(const std::vector<MixInput>& inputs, bool normalize = true);
	virtual ~Mixer() = default;
protected:
	void isValidRequest(const std::vector<MixInput>& inputs);
};