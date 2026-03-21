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

/***
 * bandlimited square wave — additive synthesis
 *
 * THEORY:
 *   a perfect square wave contains infinite odd harmonics:
 *   square(t) = sin(f) + sin(3f)/3 + sin(5f)/5 + sin(7f)/7 + ...
 *
 *   sampling an infinite series causes aliasing — all harmonics above
 *   nyquist fold back into the audible range as harsh distortion.
 *   the fix: only sum harmonics below the nyquist limit (sample_rate / 2).
 *
 * STOPPING CONDITION:
 *   stop adding harmonics when: harmonic_frequency > sample_rate / 2
 *   i.e. only include harmonic n where: f * n <= sample_rate / 2
 *
 * EXAMPLE:
 *   f = 440 Hz, sample_rate = 44100 Hz, nyquist = 22050 Hz
 *   harmonics: 440, 1320, 2200, ... up to the last odd multiple below 22050
 *
 * CLIPPING:
 *   summing many harmonics can push the signal above 1.0 or below -1.0
 *   normalize the output after summing all harmonics to keep it in [-1.0, 1.0]
 *   normalization: divide every sample by the maximum absolute value found
 *
 * IMPLEMENTATION:
 *   for each sample:
 *     sample = 0
 *     for each odd harmonic n (1, 3, 5, ...) where f*n <= nyquist:
 *       sample += sin(n * phase) / n
 *     store sample * amplitude
 *   advance phase once per frame (channel interleaving applies)
 *   normalize entire buffer after loop
 *	normalisation 
 *		- divide each sample in the buffer by the maximum absolute value
 *		- this scales the whole signal down so the loudest point is exactly 1.0 and nothing clips
 * 
 * CONTRAST WITH NAIVE:
 *   naive:       hard jump at phase = π → aliasing above nyquist
 *   bandlimited: smooth sum of sines   → no aliasing, cleaner sound
 ***/

AudioBuffer SquareWave::generate(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {
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
	const size_t channel_count_sz = static_cast<size_t>(channel_count);
	double max_sample_abs_value = 0.0;
	const double nyquist_limit = sample_rate / 2.0;
	std::vector<Harmonic> harmonics;
	int max_harmonic = static_cast<int>(nyquist_limit / frequency);
	if (max_harmonic % 2 == 1) {
		max_harmonic = max_harmonic / 2 + 1;
	}
	else {
		max_harmonic = max_harmonic / 2;
	}
	harmonics.resize(max_harmonic);

	for (size_t i = 0, j=1; i < harmonics.size(); i++, j+=2) {
		harmonics[i].amplitude = 1.0 / j;
		harmonics[i].phase_increment = 2 * std::numbers::pi * frequency * j / sample_rate;
		harmonics[i].phase = 0.0;
	}

	for (size_t i = 0; i < sample_count; i++) {
		double final_sample = 0.0;

		for (auto& h : harmonics) {
			final_sample += h.amplitude * std::sin(h.phase);
		}

		square_wave_buffer.samples[i] = final_sample;

		if (std::abs(final_sample) > max_sample_abs_value) {
			max_sample_abs_value = std::abs(final_sample);
		}

		if (i % channel_count_sz == channel_count_sz - 1) { // channel_interleaving
			for (auto& h : harmonics) {
				// fmod fixes the intensifying effect of noise over time
				h.phase = std::fmod(h.phase + h.phase_increment, 2 * std::numbers::pi);
			}
		}
	}

	for (size_t i = 0; i < sample_count; i++) {
		square_wave_buffer.samples[i] = (square_wave_buffer.samples[i] / max_sample_abs_value) * amplitude;
	}

	return square_wave_buffer;
}