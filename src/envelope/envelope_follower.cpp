#include <cmath>
#include "atae/envelope/envelope_follower.h"

void EnvelopeFollower::prepare(int sr, EnvelopeFollowerMode follower_mode) {
    sample_rate = sr;
    mode = follower_mode;
}

void EnvelopeFollower::setAttack(double attack_ms) {
    double attack_s = attack_ms / 1000.0;
    alpha_attack = 1 - std::exp(-1/(attack_s * sample_rate));
}

void EnvelopeFollower::setRelease(double release_ms) {
    double release_s = release_ms / 1000.0;
    alpha_release = 1 - std::exp(-1/(release_s * sample_rate));
}

/*
 * PEAK: y[n]= (α * |x[n]|) + ((1−α) * y[n−1])
 * RMS: y[n]= ((α * x[n]^2) + ((1−α) * y[n−1]^2))^1/2
 */
double EnvelopeFollower::processSample(double input) {
    double result = 0.0;
    double alpha = std::abs(input) > previous_output ? alpha_attack : alpha_release;
    
    if(mode == EnvelopeFollowerMode::Peak) {
        result = (alpha * std::abs(input)) + ((1 - alpha) * previous_output);
    } else {
        double input_squared = input * input;
        double previous_output_squared = previous_output * previous_output;
        result = std::pow(((alpha * input_squared) + ((1 - alpha) * previous_output_squared)), 0.5);
    }

    previous_output = result;
    return result;
}

void EnvelopeFollower::reset() {
    previous_output = 0.0;
}
