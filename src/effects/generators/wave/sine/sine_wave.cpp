#include "sine_wave.h"
#include <cmath>
#include <numbers>
#include <stdexcept>

/*
* y(t) = A * sin(2π * f * t + φ)
* Where:
*	A - amplitude (volume, between 0.0, 1.0)
*	f - frequency in Hz
*	t - time in seconds
*	φ - phase offset in radians - controls where in the cycle we start
*
* dt = 1 / sample_rate => t = i * dt = i / sample_rate => sample[i] = A * sin(2π * f * i / sample_rate)
*
*/

AudioBuffer SineWave::generate_naive(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {
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

	AudioBuffer sine_wave_buffer;
	sine_wave_buffer.channels = channel_count;
	sine_wave_buffer.sampleRate = sample_rate;

	const size_t sample_count = static_cast<size_t>(duration_s) * sample_rate * channel_count;

	sine_wave_buffer.samples.resize(sample_count);
	const size_t channel_count_sz = static_cast<size_t>(channel_count);
	for (size_t i = 0; i < sample_count; i++) {
		sine_wave_buffer.samples[i] = amplitude * std::sin(2 * std::numbers::pi * frequency * (static_cast<double>(i/ channel_count_sz) / sample_rate)); // i/channel_count is required to accumulate for channel interleaving
	}

	return sine_wave_buffer;
}

/*
* What happens when we generate a very long sine wave (maybe around 10 minutes at 44.1kHz)?
* -> The value of i gets very large which makes i/sample_rate very large
*	 Mathematically, it's fine since sine is periodic however float and double have their limitations
*	 As the number gets larger, the sine wave slowly starts drifting our of tune
* 
* To fix this, phase accumulation is used
* Instead of computing t from i every sample, a running phase variable is kept and incremented by a fixed amount each sample
*   phase_increment = 2π * f / sample_rate
*   phase = 0.0
*   each sample:
*       sample[i] = A * sin(phase)
*       phase += phase_increment
*       if phase >= 2π: phase -= 2π  ← keep phase in [0, 2π]
*
*/
AudioBuffer SineWave::generate(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {
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

	AudioBuffer sine_wave_buffer;
	sine_wave_buffer.channels = channel_count;
	sine_wave_buffer.sampleRate = sample_rate;

	const size_t sample_count = static_cast<size_t>(duration_s) * sample_rate * channel_count;

	sine_wave_buffer.samples.resize(sample_count);
	double phase_increment = 2 * std::numbers::pi * frequency / sample_rate, phase = 0.0;
	const size_t channel_count_sz = static_cast<size_t>(channel_count);
	for (size_t i = 0; i < sample_count; i++) {
		sine_wave_buffer.samples[i] = amplitude * std::sin(phase);
		if (i % channel_count_sz == channel_count_sz - 1) { // channel_interleaving
			phase += phase_increment;

			if (phase >= 2 * std::numbers::pi) {
				phase -= 2 * std::numbers::pi;
			}
		}
	}

	return sine_wave_buffer;
}
