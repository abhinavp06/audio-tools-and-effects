#pragma once
#include <vector>
#include "atae/envelope/envelope_follower.h"
#include "atae/types/audio_buffer.h"

class Limiter {
public:
  void prepare(int sr, int channels);
  void setThreshold(double threshold_db);
  void setRelease(double release_ms);
  void setMakeupGain(double gain_db);
  void apply(AudioBuffer &buffer);
  void reset();

private:
  int channel_count = 0;
  int sample_rate = 0;
  double threshold = 0.0;
  double alpha_release = 0.0;
  double alpha_attack = 0.0;
  double makeup_gain = 0.0;
  std::vector<EnvelopeFollower> envelope_followers;
  std::vector<double> gain_reduction_smoother_history;
};
