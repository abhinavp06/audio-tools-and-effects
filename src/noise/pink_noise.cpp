#include <stdexcept>
#include "atae/noise/pink_noise.h"

AudioBuffer PinkNoise::generate(const int duration_s, const int sample_rate, const int channel_count, double amplitude) {
	if (duration_s <= 0) {
		throw std::invalid_argument("Duration must be a positive integer!");
	}
	if (channel_count <= 0) {
		throw std::invalid_argument("Channel count must be a positive integer!");
	}
	if (amplitude < 0.0 || amplitude > 1.0) {
		throw std::invalid_argument("Amplitude must be between 0.0 and 1.0!");
	}
	
	const int total_samples = duration_s * sample_rate * channel_count;

	AudioBuffer pink_noise;
	pink_noise.channels = channel_count;
	pink_noise.sampleRate = sample_rate;
	pink_noise.samples.resize(total_samples);

	std::vector<double> rows(NUM_ROWS, 0.0);

	std::mt19937 engine{ std::random_device{}() };
	std::uniform_real_distribution<double> dist{ -1.0, 1.0 };

	for (int ch = 0; ch < channel_count;ch++) {
		double running_sum = 0.0;

		for (int row = 0; row < NUM_ROWS; row++) {
			rows[row] = dist(engine);
			running_sum += rows[row];
		}

		uint32_t n = 0;

		for (int frame = 0;frame < total_samples / channel_count; frame++) {
			n++;
			uint32_t changed_bits = n ^ (n - 1);
			for (int i = 0;i < NUM_ROWS;i++) {
				if (changed_bits & (1u << i)) {
					running_sum -= rows[i];
					rows[i] = dist(engine);
					running_sum += rows[i];
				}
			}

			double pink = running_sum / NUM_ROWS;

			pink *= amplitude;

			pink_noise.samples[frame * channel_count + ch] = pink;
		}
	}

	return pink_noise;
}