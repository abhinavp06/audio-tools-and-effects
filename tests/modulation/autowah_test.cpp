#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include "atae/filters/biquad/biquad_filter.h"
#include "atae/oscillators/square_wave.h"
#include "atae/oscillators/sine_wave.h"
#include "atae/modulation/lfo.h"
#include <atae/io/audio_file.h>

int main() {
	std::cout << "----- AUTOWAH by abhinavp06 -----" << std::endl;

	// WAVE
	int wave_duration_s = 10, sample_rate = 44100, channel_count = 2;
	double osc_freq = 440.0, osc_amplitude = 0.8;

	// LFO
	double lfo_rate_hz = 8.0, lfo_depth = 1.0;

	// BIQUAD FILTER
	double bf_qf = 0.707, bf_db_gain = 6.0;

	// COMMON
	double f_center = 800.0;
	double f_depth = 600.0;
	double nyquist = sample_rate / 2.0;

	try {
		SineWave osc;
		AudioBuffer osc_wave = osc.generate(wave_duration_s, sample_rate, channel_count, osc_freq, osc_amplitude);
		AudioFile::save(OUTPUT_DIR "autowah_dry.wav", osc_wave);

		Lfo lfo;
		lfo.prepare(sample_rate);
		lfo.setRate(lfo_rate_hz);
		lfo.setDepth(lfo_depth);
		lfo.setWaveform(LfoWaveform::Sine);

		BiquadFilter bf;
		bf.prepare(osc_wave.sampleRate, osc_wave.channels);
		

		int total_frames = sample_rate * wave_duration_s;
		for (int frame = 0; frame < total_frames; frame++) {
			double lfo_output = lfo.process();
			double cutoff_freq = std::clamp((f_center + f_depth * lfo_output), 20.0, nyquist - 1.0); // Added nyquist - 1.0 because => setCoefficients() throws when cutoff >= sample_rate / 2.0. std::clamp can return exactly nyquist — and nyquist >= nyquist is true. So if the LFO ever pushes cutoff to the ceiling, setCoefficients() throws and the loop dies.
			bf.setCoefficients(BiquadFilterType::LPF, cutoff_freq, bf_qf, bf_db_gain);

			for (int ch = 0; ch < channel_count; ch++) {
				size_t index = (frame * channel_count) + ch;
				osc_wave.samples[index] = bf.processSample(osc_wave.samples[index], ch);
			}
		}


		bf.reset();

		AudioFile::save(OUTPUT_DIR "autowah_wet.wav", osc_wave);

	}
	catch (const std::exception& e) {
		std::cerr << "[Error] " << e.what() << "\n";
		return 1;
	}

	std::cout << "----- GOODBYE -----" << std::endl;

	return 0;
}

/**
* My observations (wet signal):
* 1. SQUARE WAVE: 
*		- constant faint low sound
*		- like a bassline sound (not a bass.. just a faint high pitch sound)
*		- then I have a beep in some set interval (which i am assuming is the peak and trough of the square wave)
* 2. SINE WAVE:
*		- can get a sense of the "wah"
*		- "wah" will be more pronounced if we have multiple waves
*		a. SET LFO_RATE_HZ to 0.5:
*			- pretty much the same in terms of the sound
*			- the wah is so slow it barely reads as a wah
*			- however the modulation takes more time.. it's not as quick as it was with 1.0
*		b. SET LFO_RATE_HZ to 4.0:
*			- same as above.. just faster modulation
*			- it starts to feel rhythmic and vocal
*		c. SET LFO_RATE_HZ to 8.0:
*			- same as above.. even faster modulation
*			- it approaches a warble
*/