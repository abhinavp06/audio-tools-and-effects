#include <iostream>
#include <stdexcept>
#include <vector>

#include "atae/delay/feedback_delay.h"
#include "atae/oscillators/square_wave.h"
#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"

int main() {
    std::cout << "----- FEEDBACK DELAY TEST by abhinavp06 -----" << std::endl;

    int duration_s = 5;
    int sample_rate = 44100;
    int channel_count = 2;

    double frequency = 440.0;
    double amplitude = 0.8;

    float delay_ms = 300.0f;
    float feedback = 0.5f;
    float wet_mix = 0.5f;

    try {
        SquareWave square_osc;
        AudioBuffer square_wave = square_osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);
        AudioFile::save(OUTPUT_DIR "dry_square_wave.wav", square_wave);

        std::vector<FeedbackDelay> feedback_delays(channel_count);

        for (int ch = 0; ch < channel_count; ch++) {
            feedback_delays[ch].prepare(sample_rate, 1000.0f);
            feedback_delays[ch].setDelay(delay_ms);
            feedback_delays[ch].setFeedback(feedback);
            feedback_delays[ch].setWetMix(wet_mix);
        }

        AudioBuffer wet_buffer = square_wave;

        int total_frames = sample_rate * duration_s;

        for (int frame = 0; frame < total_frames; frame++) {
            for (int ch = 0; ch < channel_count; ch++) {
                size_t index = (frame * channel_count) + ch;
                wet_buffer.samples[index] = feedback_delays[ch].processSample(wet_buffer.samples[index]);
            }
        }

        AudioFile::save(OUTPUT_DIR "wet_feedback_delay_square_wave.wav", wet_buffer);
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    std::cout << "----- GOODBYE -----" << std::endl;
    return 0;
}