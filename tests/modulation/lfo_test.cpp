#include <iostream>
#include <vector>
#include "atae/modulation/lfo.h"
#include "csv_writer.h"

int main() {
    std::cout << "----- LFO TEST (CSV EXPORT) by abhinavp06 -----" << std::endl;

    int sample_rate = 44100;
    double rate_hz = 1.0;
    double depth = 1.0;

    try {
        CsvWriter writer;
        writer.open(OUTPUT_DIR "lfo_shapes.csv");

        writer.writeHeader({
            "Sine",
            "Square",
            "Saw",
            "Triangle"
        });

        Lfo sine_lfo;
        sine_lfo.prepare(sample_rate);
        sine_lfo.setRate(rate_hz);
        sine_lfo.setDepth(depth);
        sine_lfo.setWaveform(LfoWaveform::Sine);

        Lfo square_lfo;
        square_lfo.prepare(sample_rate);
        square_lfo.setRate(rate_hz);
        square_lfo.setDepth(depth);
        square_lfo.setWaveform(LfoWaveform::Square);

        Lfo saw_lfo;
        saw_lfo.prepare(sample_rate);
        saw_lfo.setRate(rate_hz);
        saw_lfo.setDepth(depth);
        saw_lfo.setWaveform(LfoWaveform::Saw);

        Lfo triangle_lfo;
        triangle_lfo.prepare(sample_rate);
        triangle_lfo.setRate(rate_hz);
        triangle_lfo.setDepth(depth);
        triangle_lfo.setWaveform(LfoWaveform::Triangle);

        int total_samples = static_cast<int>(sample_rate / rate_hz);

        for (int i = 0; i < total_samples; i++) {
            writer.writeRow({
                sine_lfo.process(),
                square_lfo.process(),
                saw_lfo.process(),
                triangle_lfo.process()
            });
        }

        writer.close();
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    std::cout << "----- GOODBYE -----" << std::endl;

    return 0;
}