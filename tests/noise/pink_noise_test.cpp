#include <iostream>
#include <stdexcept>
#include "atae/noise/pink_noise.h"
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"

int main() {
	std::cout << "----- PINK NOISE GENERATOR by abhinavp06 -----" << std::endl;

	int duration_s, sample_rate, channel_count;
	double amplitude;

	std::cout << "Enter the duration: \n";
	std::cin >> duration_s;

	std::cout << "Enter the sample rate: \n";
	std::cin >> sample_rate;

	std::cout << "Enter the number of channels: \n";
	std::cin >> channel_count;

	std::cout << "Enter the amplitude: \n";
	std::cin >> amplitude;

	try {
		AudioBuffer pink_noise = PinkNoise::generate(duration_s, sample_rate, channel_count, amplitude);
		AudioFile::save(OUTPUT_DIR "pink_noise_output.wav", pink_noise);
	}
	catch (const std::exception& e) {
		std::cerr << "[Error] " << e.what() << "\n";
		return 1;
	}

	std::cout << "----- GOODBYE -----" << std::endl;

	return 0;
}