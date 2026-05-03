#include <iostream>
#include <vector>

#include "atae/types/audio_buffer.h"
#include "atae/io/audio_file.h"
#include "atae/oscillators/sine_wave.h"
#include "atae/envelope/envelope_follower.h"
#include "csv_writer.h"

int main() {
    std::cout << "----- ENVELOPE FOLLOWER TEST by abhinavp06 -----" << std::endl;

    int duration_s = 5;
    int sample_rate = 44100;
    int channel_count = 2;

    double frequency = 440.0;
    double amplitude = 0.8;

    try {
        SineWave osc;
        AudioBuffer buffer = osc.generate(duration_s, sample_rate, channel_count, frequency, amplitude);

        AudioBuffer processed_buffer = buffer;

        EnvelopeFollower peakFollower;
        EnvelopeFollower rmsFollower;

        peakFollower.prepare(sample_rate, EnvelopeFollowerMode::Peak);
        rmsFollower.prepare(sample_rate, EnvelopeFollowerMode::RMS);

        peakFollower.setAttack(10.0);
        rmsFollower.setAttack(10.0);

        peakFollower.setRelease(100.0);
        rmsFollower.setRelease(100.0);

        CsvWriter csv;
        csv.open(OUTPUT_DIR "envelope_follower.csv");
        csv.writeHeader({ "raw_sample", "peak_env", "rms_env" });

        int total_frames = duration_s * sample_rate;

        for (int frame = 0; frame < total_frames; frame++) {
            int base_index = frame * channel_count;

            double left_sample = buffer.samples[base_index];

            double peak_env = peakFollower.processSample(left_sample);
            double rms_env  = rmsFollower.processSample(left_sample);

            csv.writeRow({ left_sample, peak_env, rms_env });

            processed_buffer.samples[base_index] *= peak_env;
            processed_buffer.samples[base_index + 1] *= peak_env;
        }

        csv.close();

        AudioFile::save(OUTPUT_DIR "sine_original.wav", buffer);
        AudioFile::save(OUTPUT_DIR "sine_peak_env_applied.wav", processed_buffer);

    }
    catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    std::cout << "----- GOODBYE -----" << std::endl;
    return 0;
}