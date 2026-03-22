#include <iostream>
#include <stdexcept>
#include <vector>
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"
#include "atae/noise/white_noise.h"
#include "atae/oscillators/saw_wave.h"
#include "atae/oscillators/sine_wave.h"
#include "atae/oscillators/square_wave.h"
#include "atae/mixer/mixer.h"

int main() {
	std::cout << "----- MIXER by abhinavp06 -----" << std::endl;

	int duration_s = 60, sample_rate = 44100, channel_count = 2;

	try {
		AudioBuffer white_noise = WhiteNoise::generate(duration_s, sample_rate, channel_count);
		SawWave saw_wave;
		SineWave sine_wave;
		SquareWave square_wave;

		AudioBuffer saw_wave_buffer = saw_wave.generate(duration_s, sample_rate, channel_count, 666, 0.6);
		AudioBuffer sine_wave_buffer = sine_wave.generate(duration_s, sample_rate, channel_count, 666, 0.6);
		AudioBuffer square_wave_buffer = square_wave.generate(duration_s, sample_rate, channel_count, 666, 0.6);

		std::vector<MixInput> mix_inputs = { {white_noise, 0.3}, {saw_wave_buffer, 0.4}, {sine_wave_buffer, 0.5}, {square_wave_buffer, 0.6} };

		Mixer mixer;
		AudioBuffer mix = mixer.mix(mix_inputs, true);
		AudioFile::save(OUTPUT_DIR "mix.wav", mix);
	}
	catch (const std::exception& e) {
		std::cerr << "[Error] " << e.what() << "\n";
		return 1;
	}

	std::cout << "----- GOODBYE -----" << std::endl;

	return 0;
}