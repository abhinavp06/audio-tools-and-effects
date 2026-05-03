#include "atae/oscillators/saw_wave.h"
#include "atae/types/audio_buffer.h"
#include "atae/types/harmonic.h"
#include <cmath>
#include <numbers>
#include <stdexcept>

/*
* y(t) = sin(f) - sin(2f)/2 + sin(3f)/3 - sin(4f)/4 + sin(5f)/5 - sin(4f)/4 + sin(7f)/7 - ...
*/

AudioBuffer SawWave::generate_naive(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {
	isValidRequest(duration_s, sample_rate, channel_count, frequency, amplitude);
	AudioBuffer saw_wave_buffer = setupAudioBuffer(duration_s, sample_rate, channel_count);

	double phase_increment = 2 * std::numbers::pi * frequency / sample_rate, phase = 0.0;
	const size_t channel_count_sz = static_cast<size_t>(channel_count);
	for (size_t i = 0; i < saw_wave_buffer.samples.size(); i++) {
		saw_wave_buffer.samples[i] = (amplitude / std::numbers::pi) * phase - amplitude;
		if (i % channel_count_sz == channel_count_sz - 1) { // channel_interleaving
			phase += phase_increment;

			if (phase >= 2 * std::numbers::pi) {
				phase -= 2 * std::numbers::pi;
			}
		}
	}

	return saw_wave_buffer;
}

AudioBuffer SawWave::generate(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {
	isValidRequest(duration_s, sample_rate, channel_count, frequency, amplitude);
	AudioBuffer saw_wave_buffer = setupAudioBuffer(duration_s, sample_rate, channel_count);
	const size_t channel_count_sz = static_cast<size_t>(channel_count);
	double max_sample_abs_value = 0.0;
	const double nyquist_limit = sample_rate / 2.0;
	std::vector<Harmonic> harmonics;
	int max_harmonic = static_cast<int>(nyquist_limit / frequency);
	harmonics.resize(max_harmonic);
	double sign = 1.0;
	for (size_t i = 0, j = 1; i < harmonics.size(); i++, j ++) {
		harmonics[i].amplitude = sign / j;
		harmonics[i].phase_increment = 2 * std::numbers::pi * frequency * j / sample_rate;
		harmonics[i].phase = 0.0;
		sign *= -1.0;
	}

	for (size_t i = 0; i < saw_wave_buffer.samples.size(); i++) {
		double final_sample = 0.0;

		for (auto& h : harmonics) {
			final_sample += h.amplitude * std::sin(h.phase);
		}

		saw_wave_buffer.samples[i] = final_sample;

		if (std::abs(final_sample) > max_sample_abs_value) {
			max_sample_abs_value = std::abs(final_sample);
		}

		if (i % channel_count_sz == channel_count_sz - 1) {
			for (auto& h : harmonics) {
				h.phase = std::fmod(h.phase + h.phase_increment, 2 * std::numbers::pi);
			}
		}
	}

	for (size_t i = 0; i < saw_wave_buffer.samples.size(); i++) {
		saw_wave_buffer.samples[i] = (saw_wave_buffer.samples[i] / max_sample_abs_value) * amplitude;
	}

	return saw_wave_buffer;
}