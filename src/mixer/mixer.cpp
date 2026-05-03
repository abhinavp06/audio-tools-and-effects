#include <vector>
#include <stdexcept>
#include <cmath>
#include "atae/mixer/mixer.h"
#include "atae/types/audio_buffer.h"

void Mixer::isValidRequest(const std::vector<MixInput>& inputs) {
	if (inputs.size() == 0) {
		throw std::invalid_argument("Atleast 1 mixer input required!");
	}

	const int init_sample_rate = inputs[0].audio_buffer.sampleRate, init_channel_count = inputs[0].audio_buffer.channels;

	for (int i = 0; i < inputs.size(); i++) {
		if (inputs[i].gain < 0.0 || inputs[i].gain > 1.0) {
			throw std::invalid_argument("Gain must be between 0.0 and 1.0");
		}
		if (i > 0) {
			if (inputs[i].audio_buffer.sampleRate != init_sample_rate) {
				throw std::invalid_argument("Sample rates must be same for all inputs!");
			}
			if (inputs[i].audio_buffer.channels != init_channel_count) {
				throw std::invalid_argument("Channel count must be same for all inputs!");
			}
			if (inputs[i].audio_buffer.samples.size() != inputs[0].audio_buffer.samples.size()) {
				throw std::invalid_argument("All inputs must have the same length");
			}
		}
	}
}

AudioBuffer Mixer::mix(const std::vector<MixInput>& inputs, bool normalize) {
	isValidRequest(inputs);

	AudioBuffer result_buffer;
	const size_t sample_count = inputs[0].audio_buffer.samples.size();
	if (sample_count == 0) {
		throw std::invalid_argument("Sample count cannot be zero!");
	}

	result_buffer.samples.resize(sample_count);
	result_buffer.channels = inputs[0].audio_buffer.channels;
	result_buffer.sampleRate = inputs[0].audio_buffer.sampleRate;

	double final_sample, max_sample_abs_value = 0.0;
	for (int i = 0; i < sample_count; i++) {
		final_sample = 0.0;
		for (int j = 0; j < inputs.size();j++) {
			final_sample += inputs[j].audio_buffer.samples[i] * inputs[j].gain;
			if (std::abs(final_sample) > max_sample_abs_value) {
				max_sample_abs_value = std::abs(final_sample);
			}
		}

		result_buffer.samples[i] = final_sample;
	}

	if (normalize) {
		for (size_t i = 0; i < result_buffer.samples.size(); i++) {
			result_buffer.samples[i] = (result_buffer.samples[i] / max_sample_abs_value);
		}
	}

	return result_buffer;
}