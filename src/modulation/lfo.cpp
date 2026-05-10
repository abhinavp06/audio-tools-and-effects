#include "atae/modulation/lfo.h"
#include <cmath>
#include <numbers>

void Lfo::setWaveform(LfoWaveform waveform_type) { waveform = waveform_type; }

void Lfo::prepare(int sr) {
  sample_rate = sr;
  phase = 0.0;
}

void Lfo::setRate(double hz) {
  rate = hz;
  phase_increment = 2 * std::numbers::pi * hz / sample_rate;
}

void Lfo::setDepth(double depth) { _depth = depth; }

double Lfo::process() {
  double result = 0.0;

  switch (waveform) {
  case LfoWaveform::Sine:
    result = std::sin(phase);
    break;
  case LfoWaveform::Square:
    result = (sin(phase) >= 0.0) ? 1.0 : -1.0;
    break;
  case LfoWaveform::Saw: {
    double normalized_phase = phase / (2 * std::numbers::pi);
    result = 2 * normalized_phase - 1.0;
    break;
  }
  case LfoWaveform::Triangle:
    result = (2.0 / std::numbers::pi) * (std::asin(std::sin(phase)));
    break;
  }

  phase += phase_increment;

  if (phase >= 2 * std::numbers::pi) {
    phase -= 2 * std::numbers::pi;
  }

  return result * _depth;
}
