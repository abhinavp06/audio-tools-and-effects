#include <iostream>
#include "atae/dynamics/limiter.h"
#include "atae/envelope/adsr.h"
#include "atae/io/audio_file.h"
#include "atae/oscillators/sine_wave.h"
#include "atae/types/audio_buffer.h"

int main() {
    std::cout << "----- LIMITER TEST (WITH ADSR) by abhinavp06 -----" << std::endl;

    int duration_s = 10;
    int sample_rate = 44100;
    int channel_count = 2;

    double frequency = 440.0;
    double amplitude = 0.95;

    double attack_s = 1.0;
    double decay_s = 1.5;
    double sustain_level = 0.3;
    double release_s = 2.0;

    try {
        SineWave osc;
        AudioBuffer buffer = osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);

        Adsr adsr;
        adsr.setAttack(attack_s, sample_rate);
        adsr.setDecay(decay_s, sample_rate);
        adsr.setSustain(sustain_level);
        adsr.setRelease(release_s, sample_rate);

        adsr.noteOn();

        int total_frames = duration_s * sample_rate;
        int note_off_frame = (duration_s - static_cast<int>(release_s)) * sample_rate;

        for (int frame = 0; frame < total_frames; frame++) {
            double env_value = adsr.process();

            for (int ch = 0; ch < channel_count; ch++) {
                size_t index = (frame * channel_count) + ch;
                buffer.samples[index] *= env_value;
            }

            if (frame == note_off_frame) {
                adsr.noteOff();
            }
        }

        AudioFile::save(OUTPUT_DIR "dry_sine_adsr_limiter.wav", buffer);

        AudioBuffer wet_buffer = buffer;

        Limiter limiter;
        limiter.prepare(sample_rate, channel_count);
        limiter.setThreshold(-18.0);
        limiter.setRelease(100.0);
        limiter.setMakeupGain(6.0);

        limiter.apply(wet_buffer);

        AudioFile::save(OUTPUT_DIR "wet_sine_adsr_limited.wav", wet_buffer);
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    std::cout << "----- GOODBYE -----" << std::endl;
    return 0;
}