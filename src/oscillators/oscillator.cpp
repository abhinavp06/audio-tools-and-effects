#include "atae/oscillators/oscillator.h"
#include "atae/types/audio_buffer.h"
#include <stdexcept>

void Oscillator::isValidRequest(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {
	if (duration_s <= 0) {
		throw std::invalid_argument("Duration must be a positive integer!");
	}
	if (channel_count <= 0) {
		throw std::invalid_argument("Channel count must be a positive integer!");
	}
	if (frequency > static_cast<double>(sample_rate) / 2) {
		throw std::invalid_argument("Frequency must be greater than or equal to half of the sample rate!");
	}
	if (amplitude < 0.0 || amplitude > 1.0) {
		throw std::invalid_argument("Amplitude must be between 0.0 and 1.0!");
	}
}

AudioBuffer Oscillator::setupAudioBuffer(const int duration_s, const int sample_rate, const int channel_count) {
	AudioBuffer new_buffer;

	const size_t sample_count = static_cast<size_t>(duration_s) * sample_rate * channel_count;
	if (sample_count == 0) {
		throw std::invalid_argument("Sample count cannot be zero!");
	}

	new_buffer.samples.resize(sample_count);
	new_buffer.channels = channel_count;
	new_buffer.sampleRate = sample_rate;

	return new_buffer;
}