#include <iostream>
#include <stdexcept>
#include "atae/noise/white_noise.h"

int main() {
	std::cout << "----- WHITE NOISE GENERATOR by abhinavp06 -----" << std::endl;

	int duration_s, sample_rate, channel_count;

	std::cout << "Enter the duration: \n";
	std::cin >> duration_s;

	std::cout << "Enter the sample rate: \n";
	std::cin >> sample_rate;

	std::cout << "Enter the number of channels: \n";
	std::cin >> channel_count;

	try {
		AudioBuffer white_noise = WhiteNoise::generate(duration_s, sample_rate, channel_count);
		AudioFile::save("white_noise_output.wav", white_noise);
	}
	catch (const std::exception& e) {
		std::cerr << "[Error] " << e.what() << "\n";
		return 1;
	}

	std::cout << "----- GOODBYE -----" << std::endl;

	return 0;
}

/**
* Why use "/n" instead of std::endl?
* -> std::endl flushes the buffer every time (flush - push everything in the buffer to the actual output device right now" 
*	 std::endl does 2 things - write "/n" and flush
*    flushing is a system call and can become slower in a large scale project
*	 best to use it when for example there is a crash prone operation where we want to guarantee the output appeared
* 
*/