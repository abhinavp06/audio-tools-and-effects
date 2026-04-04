#include <iostream>
#include <stdexcept>
#include <vector>
#include "atae/filters/comb/comb_filter.h"
#include "atae/oscillators/square_wave.h"
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"

int main() {
    std::cout << "----- COMB FILTER TEST by abhinavp06 -----" << std::endl;

    int duration_s = 5;
    int sample_rate = 44100;
    int channel_count = 2;

    double frequency = 440.0;
    double amplitude = 0.8;

    float delay_ms = 10.0f;
    float gain = 0.7f;

    try {
        SquareWave square_osc;
        AudioBuffer square_wave = square_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
        AudioFile::save(OUTPUT_DIR "dry_square_wave.wav", square_wave);

        std::vector<CombFilter> comb_filters(channel_count);

        for (int ch = 0; ch < channel_count; ch++) {
            comb_filters[ch].prepare(sample_rate, 1000.0f);
            comb_filters[ch].setDelay(delay_ms);
            comb_filters[ch].setGain(gain);
        }

        AudioBuffer comb_filtered_buffer = square_wave;

        int total_frames = sample_rate * duration_s;

        for (int frame = 0; frame < total_frames; frame++) {
            for (int ch = 0; ch < channel_count; ch++) {
                size_t index = (frame * channel_count) + ch;
                comb_filtered_buffer.samples[index] =
                    comb_filters[ch].processSample(comb_filtered_buffer.samples[index]);
            }
        }

        AudioFile::save(OUTPUT_DIR "wet_comb_filter_square_wave.wav", comb_filtered_buffer);
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    std::cout << "----- GOODBYE -----" << std::endl;
    return 0;
}