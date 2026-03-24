#include <cmath>
#include <numbers>
#include <stdexcept>
#include <algorithm>
#include "atae/filters/low_pass/one_pole_lpf.h"

/**
* ωc​ = 2π * (fc/​fs)
*​​ α = wc / (wc + 1)​​
*/
void OnePoleLpf::setCutoff(double cutoff_frequency, int sample_rate, int channel_count) {
    if (cutoff_frequency <= 0.0 || cutoff_frequency >= sample_rate / 2.0) {
        throw std::invalid_argument("Cutoff must be > 0 and < Nyquist!");
    }
    const double wc = 2.0 * std::numbers::pi * cutoff_frequency / sample_rate;
    blendCoefficient = wc / (wc + 1.0);
    oneMinusBlendCoefficient = 1.0 - blendCoefficient;

    previousOutputPerChannel.resize(channel_count);
    std::fill(previousOutputPerChannel.begin(), previousOutputPerChannel.end(), 0);
}

// y[n] = α * x[n] + (1 − α) * y[n−1]
void OnePoleLpf::apply(AudioBuffer& buffer) {
    for (int ch = 0; ch < buffer.channels; ch++) {
        for (size_t i = ch; i < buffer.samples.size(); i += buffer.channels) {
            auto& sample = buffer.samples[i];

            sample = blendCoefficient * sample
                + oneMinusBlendCoefficient * previousOutputPerChannel[ch];

            previousOutputPerChannel[ch] = sample;
        }
    }
}

/*
* this can be integrated in the apply function above directly after the for loop ends
* but i am making it explicit so that if i ever send buffers in chunks (maybe if its a very long signal), i have the state
*/
void OnePoleLpf::reset() {
    for (auto& history : previousOutputPerChannel) {
        history = 0.0;
   }
}