#include <iostream>
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"
#include "atae/oscillators/sine_wave.h"
#include "atae/envelope/adsr.h"

int main() {
	std::cout << "----- ADSR ENVELOPE by abhinavp06 -----" << std::endl;


	int duration_s = 30, sample_rate = 44100, channel_count = 2;
	double frequency = 666.6, amplitude = 0.6, attack_s = 2.0, decay_s = 4.0, sustain_level = 0.2, release_s = 5.0;

	try {
		SineWave osc;
		AudioBuffer buffer = osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);

		Adsr adsr;
		adsr.setAttack(attack_s, sample_rate);
		adsr.setDecay(decay_s, sample_rate);
		adsr.setSustain(sustain_level);
		adsr.setRelease(release_s, sample_rate);

		adsr.noteOn();

		int total_samples = duration_s * sample_rate * channel_count;
		int note_off_frame = (duration_s - static_cast<int>(release_s)) * sample_rate;

		for (int i = 0; i < total_samples; i+= channel_count) {
			double processed_value = adsr.process();
			for (int j = i; j < i + channel_count; j++) {
				buffer.samples[j] *= processed_value;
			}

			if (i  == note_off_frame * channel_count) {
				adsr.noteOff();
			}
		}
		
		AudioFile::save(OUTPUT_DIR "adsr.wav", buffer);

	}
	catch (const std::exception& e) {
		std::cerr << "[Error] " << e.what() << "\n";
		return 1;
	}

	std::cout << "----- GOODBYE -----" << std::endl;

	return 0;
}