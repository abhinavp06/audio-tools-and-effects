#include <iostream>
#include <stdexcept>
#include "atae/filters/low_pass/one_pole_lpf.h"
#include "atae/oscillators/sine_wave.h"
#include "atae/oscillators/square_wave.h"
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"

int main() {
	std::cout << "----- ONE POLE LOW PASS FILTER by abhinavp06 -----" << std::endl;

	int duration_s, sample_rate, channel_count;
	double frequency, amplitude, cutoff_frequency;

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

	std::cout << "Enter the cutoff frequency: \n";
	std::cin >> cutoff_frequency;



	try {
		SineWave sine_osc;
		AudioBuffer sine_wave = sine_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
		
		SquareWave square_osc;
		AudioBuffer square_wave = square_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
		
		OnePoleLpf lpf;
		lpf.setCutoff(cutoff_frequency, sample_rate);

		AudioFile::save(OUTPUT_DIR "original_sine_wave.wav", sine_wave);
		lpf.apply(sine_wave);
		AudioFile::save(OUTPUT_DIR "filtered_sine_wave.wav", sine_wave);

		lpf.reset();

		AudioFile::save(OUTPUT_DIR "original_square_wave.wav", square_wave);
		lpf.apply(square_wave);
		AudioFile::save(OUTPUT_DIR "filtered_square_wave.wav", square_wave);

	}
	catch (const std::exception& e) {
		std::cerr << "[Error] " << e.what() << "\n";
		return 1;
	}

	std::cout << "----- GOODBYE -----" << std::endl;

	return 0;
}