#include <iostream>
#include <stdexcept>
#include <vector>

#include "atae/delay/delay_line.h"
#include "atae/oscillators/square_wave.h"
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"

int main() {
    std::cout << "----- DELAY LINE TEST by abhinavp06 -----" << std::endl;

    int duration_s = 5;
    int sample_rate = 44100;
    int channel_count = 2;

    double frequency = 440.0;
    double amplitude = 0.8;
    float delay_ms = 300.0f;

    try {
        SquareWave square_osc;
        AudioBuffer square_wave = square_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
        AudioFile::save(OUTPUT_DIR "dry_square_wave.wav", square_wave);

        std::vector<DelayLine> delay_lines(channel_count);

        for (int ch = 0; ch < channel_count; ch++) {
            delay_lines[ch].prepare(sample_rate, 1000.0f);
            delay_lines[ch].setDelay(delay_ms);
        }

        AudioBuffer delayed_buffer = square_wave;

        int total_frames = sample_rate * duration_s;

        for (int frame = 0; frame < total_frames; frame++) {
            for (int ch = 0; ch < channel_count; ch++) {
                size_t index = (frame * channel_count) + ch;
                delayed_buffer.samples[index] = delay_lines[ch].processSample(delayed_buffer.samples[index]);
            }
        }

        AudioFile::save(OUTPUT_DIR "wet_square_wave.wav", delayed_buffer);
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    std::cout << "----- GOODBYE -----" << std::endl;
    return 0;
}