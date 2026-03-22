#include <iostream>
#include <stdexcept>
#include "atae/oscillators/sine_wave.h"
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"

int main() {
	std::cout << "----- SINE WAVE GENERATOR by abhinavp06 -----" << std::endl;

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
		AudioBuffer sine_wave_naive = SineWave::generate_naive(duration_s, sample_rate, channel_count, frequency, amplitude);
		AudioFile::save("sine_wave_naive_output.wav", sine_wave_naive);

		AudioBuffer sine_wave = SineWave::generate(duration_s, sample_rate, channel_count, frequency, amplitude);
		AudioFile::save("sine_wave_output.wav", sine_wave);
	}
	catch (const std::exception& e) {
		std::cerr << "[Error] " << e.what() << "\n";
		return 1;
	}

	std::cout << "----- GOODBYE -----" << std::endl;

	return 0;
}