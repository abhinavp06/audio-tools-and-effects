#include "square_wave.h"
#include <cmath>
#include <numbers>
#include <stdexcept>

AudioBuffer SquareWave::generate_naive(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {
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

	AudioBuffer square_wave_buffer;
	square_wave_buffer.channels = channel_count;
	square_wave_buffer.sampleRate = sample_rate;

	const size_t sample_count = static_cast<size_t>(duration_s) * sample_rate * channel_count;

	square_wave_buffer.samples.resize(sample_count);
	double phase_increment = 2 * std::numbers::pi * frequency / sample_rate, phase = 0.0;
	const size_t channel_count_sz = static_cast<size_t>(channel_count);
	for (size_t i = 0; i < sample_count; i++) {
		square_wave_buffer.samples[i] = phase < std::numbers::pi ? amplitude : -amplitude;
		if (i % channel_count_sz == channel_count_sz - 1) { // channel_interleaving
			phase += phase_increment;

			if (phase >= 2 * std::numbers::pi) {
				phase -= 2 * std::numbers::pi;
			}
		}
	}

	return square_wave_buffer;
}

/*
* y(t) = sin(f) + sin(3f)/3 + sin(5f)/5 + sin(7f)/7 + ...
*/
AudioBuffer SquareWave::generate(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {
	AudioBuffer square_wave_buffer;

	return square_wave_buffer;
}