#include <cmath>
#include <numbers>
#include <stdexcept>
#include "atae/filters/low_pass/one_pole_lpf.h"

/**
* ωc​ = 2π * (fc/​fs)
*​​ α = wc / (wc + 1)​​
*/
void OnePoleLpf::setCutoff(double cutoff_frequency, int sample_rate) {
    if (cutoff_frequency <= 0.0 || cutoff_frequency >= sample_rate / 2.0) {
        throw std::invalid_argument("Cutoff must be > 0 and < Nyquist!");
    }
    const double wc = 2.0 * std::numbers::pi * cutoff_frequency / sample_rate;
    blendCoefficient = wc / (wc + 1.0);
    oneMinusBlendCoefficient = 1.0 - blendCoefficient;
}

// y[n] = α * x[n] + (1 − α) * y[n−1]
void OnePoleLpf::apply(AudioBuffer& buffer) {
    for (auto& sample : buffer.samples) {
        sample = blendCoefficient * sample + oneMinusBlendCoefficient * previousFilteredOutput;
        previousFilteredOutput = sample;
    }
}

/*
* this can be integrated in the apply function above directly after the for loop ends
* but i am making it explicit so that if i ever send buffers in chunks (maybe if its a very long signal), i have the state
*/
void OnePoleLpf::reset() {
    previousFilteredOutput = 0.0;
}