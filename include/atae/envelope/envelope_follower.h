#pragma once

enum class EnvelopeFollowerMode { Peak, RMS };

class EnvelopeFollower {
public:
  void prepare(int sr, EnvelopeFollowerMode follower_mode);
  void setAttack(double attack_ms);
  void setRelease(double release_ms);
  double processSample(double input);
  void reset();

private:
  int sample_rate = 0;
  EnvelopeFollowerMode mode = EnvelopeFollowerMode::Peak;
  double alpha_attack = 0.0;
  double alpha_release = 0.0;
  double previous_output = 0.0;
};
