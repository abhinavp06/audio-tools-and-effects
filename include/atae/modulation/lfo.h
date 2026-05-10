#pragma once

enum class LfoWaveform { Sine, Square, Saw, Triangle };

class Lfo {
public:
  void setWaveform(LfoWaveform waveform_type);
  void prepare(int sr);
  void setRate(double hz);
  void setDepth(double depth);
  double process();

private:
  LfoWaveform waveform = LfoWaveform::Sine;
  int sample_rate = 0;
  double rate = 0.0;
  double _depth = 0.0;
  double phase = 0.0;
  double phase_increment = 0.0;
};
