#include "atae/dynamics/compressor.h"
#include "atae/envelope/envelope_follower.h"
#include "atae/types/audio_buffer.h"
#include <algorithm>
#include <cassert>
#include <cmath>

void Compressor::prepare(int sr, int channels) {
  channel_count = channels;
  sample_rate = sr;

  envelope_followers.reserve(channel_count);
  gain_reduction_smoother_history.resize(channel_count);

  for (int i = 0; i < channel_count; i++) {
    EnvelopeFollower env;
    env.prepare(sample_rate, EnvelopeFollowerMode::Peak);
    envelope_followers.push_back(env);
  }
}

void Compressor::setThreshold(double threshold_db) { threshold = threshold_db; }

void Compressor::setRatio(double r) { ratio = r; }

void Compressor::setAttack(double attack_ms) {
  for (int i = 0; i < channel_count; i++) {
    envelope_followers[i].setAttack(attack_ms);
  }

  double attack_s = attack_ms / 1000.0;
  alpha_attack = 1 - std::exp(-1 / (attack_s * sample_rate));
}

void Compressor::setRelease(double release_ms) {
  for (int i = 0; i < channel_count; i++) {
    envelope_followers[i].setRelease(release_ms);
  }

  double release_s = release_ms / 1000.0;
  alpha_release = 1 - std::exp(-1 / (release_s * sample_rate));
}

void Compressor::setMakeupGain(double gain_db) { makeup_gain = gain_db; }

void Compressor::apply(AudioBuffer &buffer) {
  assert(gain_reduction_smoother_history.size() == buffer.channels);

  const double epsilon = 1e-12;
  double one_minus_alpha_attack = 1 - alpha_attack;
  double one_minus_alpha_release = 1 - alpha_release;

  for (int ch = 0; ch < buffer.channels; ch++) {
    for (size_t i = ch; i < buffer.samples.size(); i += buffer.channels) {
      auto &sample = buffer.samples[i];

      double input_db =
          20 *
          std::log10(envelope_followers[ch].processSample(sample) + epsilon);

      double gain_reduction = 0.0;
      if (input_db - threshold >= 0) {
        gain_reduction = (input_db - threshold) * (1 - (1 / ratio));
      }

      if (gain_reduction > gain_reduction_smoother_history[ch]) {
        gain_reduction_smoother_history[ch] =
            alpha_attack * gain_reduction_smoother_history[ch] +
            one_minus_alpha_attack *
                gain_reduction;
      } else {
        gain_reduction_smoother_history[ch] =
            alpha_release * gain_reduction_smoother_history[ch] +
            one_minus_alpha_release * gain_reduction;
      }

      sample =
          sample *
          std::pow(10,
                   (((-1 * gain_reduction_smoother_history[ch]) + makeup_gain) /
                    20));
    }
  }
}

void Compressor::reset() {
  std::fill(gain_reduction_smoother_history.begin(),
            gain_reduction_smoother_history.end(), 0.0);

  for (int i = 0; i < channel_count; i++) {
    envelope_followers[i].reset();
  }
}
