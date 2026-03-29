#include <iostream>
#include <stdexcept>
#include "atae/filters/biquad/biquad_filter.h"
#include "atae/oscillators/square_wave.h"
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"

int main() {
	std::cout << "----- BIQUAD FILTER by abhinavp06 -----" << std::endl;

	int duration_s = 5, sample_rate = 44100, channel_count = 2;
	// change quality_factor to 8.0 to test the peaking eq filter properly
	double frequency = 440.0, amplitude = 0.8, cutoff_frequency = 1000.0, quality_factor = 0.707, db_gain = 6.0;



	try {
		SquareWave square_osc;
		AudioBuffer square_wave = square_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
		AudioFile::save(OUTPUT_DIR "original_square_wave.wav", square_wave);

		BiquadFilter bf;

		// 1. LPF
		bf.prepare(square_wave.sampleRate, square_wave.channels);
		bf.setCoefficients(BiquadFilterType::LPF, cutoff_frequency, quality_factor, 0.0);
		bf.apply(square_wave);
		AudioFile::save(OUTPUT_DIR "biquad_lpf_square_wave.wav", square_wave);
		bf.reset();
		
		// 2. HPF
		bf.prepare(square_wave.sampleRate, square_wave.channels);
		bf.setCoefficients(BiquadFilterType::HPF, cutoff_frequency, quality_factor, 0.0);
		square_wave = square_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
		bf.apply(square_wave);
		AudioFile::save(OUTPUT_DIR "biquad_hpf_square_wave.wav", square_wave);
		bf.reset();
		
		// 3. LowShelf
		bf.prepare(square_wave.sampleRate, square_wave.channels);
		bf.setCoefficients(BiquadFilterType::LowShelf, cutoff_frequency, quality_factor, db_gain);
		square_wave = square_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
		bf.apply(square_wave);
		AudioFile::save(OUTPUT_DIR "biquad_lowshelf_square_wave.wav", square_wave);
		bf.reset();
		
		// 4. HighShelf
		bf.prepare(square_wave.sampleRate, square_wave.channels);
		bf.setCoefficients(BiquadFilterType::HighShelf, cutoff_frequency, quality_factor, db_gain);
		square_wave = square_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
		bf.apply(square_wave);
		AudioFile::save(OUTPUT_DIR "biquad_highshelf_square_wave.wav", square_wave);
		bf.reset();
		
		// 5. PeakingEQ
		bf.prepare(square_wave.sampleRate, square_wave.channels);
		bf.setCoefficients(BiquadFilterType::PeakingEQ, cutoff_frequency, quality_factor, db_gain);
		square_wave = square_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
		bf.apply(square_wave);
		AudioFile::save(OUTPUT_DIR "biquad_peakingeq_square_wave.wav", square_wave);
		bf.reset();
	}
	catch (const std::exception& e) {
		std::cerr << "[Error] " << e.what() << "\n";
		return 1;
	}

	std::cout << "----- GOODBYE -----" << std::endl;

	return 0;
}