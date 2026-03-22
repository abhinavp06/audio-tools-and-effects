#include "atae/noise/white_noise.h"
#include <random>
#include <stdexcept>
#include <chrono>

AudioBuffer WhiteNoise::generate(const int duration_s, const int sample_rate, const int channel_count) {
	if (duration_s <= 0) {
		throw std::invalid_argument("Duration must be a positive integer!");
	}
	/**
	* Mono - 1 channel
	* Stereo - 2 channels - left, right - most consumer audio
	* LCR - 3 channels - left, center, right - used in film and some live sound rigs
	* Quad - 4 channels - early surround sound format
	* 5.1 - 6 channels - left, right, left surround, right surround, subwoofer - standard for film and home theater
	* 7.1 - 8 channels - extend 5.1 with 2 more surround channels - cinema, high-end game audio
	* Ambisonics - 4, 9 or 16 channels - spatial audio format for VR and immersive audio - channels represent spherical harmonics, not speaker positions
	*/
	if (channel_count <= 0) {
		throw std::invalid_argument("Channel count must be a positive integer!");
	}


	AudioBuffer white_noise_buffer;
	white_noise_buffer.channels = channel_count;
	white_noise_buffer.sampleRate = sample_rate;

	std::mt19937 engine(std::random_device{}() ^ static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

	std::uniform_real_distribution<double> dist(-1.0, 1.0);

	const size_t sample_count = static_cast<size_t>(duration_s) * sample_rate * channel_count;

	white_noise_buffer.samples.resize(sample_count);
	/*
	* don't need to care about channels here since each sample is independent random noise
	* every sample gets a fresh random value
	*/
	for (size_t i = 0; i < sample_count; i++) {
		white_noise_buffer.samples[i] = dist(engine);
	}

	return white_noise_buffer;
}