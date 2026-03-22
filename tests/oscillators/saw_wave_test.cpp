#include <iostream>
#include <stdexcept>
#include "atae/oscillators/saw_wave.h"
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"

int main() {
	std::cout << "----- SAW WAVE GENERATOR by abhinavp06 -----" << std::endl;

	int duration_s, sample_rate, channel_count;
	double frequency, amplitude;

	std::cout << "Enter the duration: \n";
	std::cin >> duration_s;

	std::cout << "Enter the sample rate: \n";
	std::cin >> sample_rate;

	std::cout << "Enter the number of channels: \n";
	std::cin >> channel_count;

	std::cout << "Enter the frequency: \n";
	std::cin >> frequency;

	std::cout << "Enter the amplitude: \n";
	std::cin >> amplitude;



	try {
		AudioBuffer saw_wave_naive = SawWave::generate_naive(duration_s, sample_rate, channel_count, frequency, amplitude);
		AudioFile::save("saw_wave_naive_output.wav", saw_wave_naive);

		AudioBuffer saw_wave = SawWave::generate(duration_s, sample_rate, channel_count, frequency, amplitude);
		AudioFile::save("saw_wave_output.wav", saw_wave);
	}
	catch (const std::exception& e) {
		std::cerr << "[Error] " << e.what() << "\n";
		return 1;
	}

	std::cout << "----- GOODBYE -----" << std::endl;

	return 0;
}