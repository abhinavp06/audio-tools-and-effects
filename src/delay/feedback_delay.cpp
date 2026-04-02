#include <stdexcept>
#include "atae/delay/feedback_delay.h"
#include "atae/delay/delay_line.h"

void FeedbackDelay::prepare(int sample_rate, float max_delay_ms) {
	delay_line.prepare(sample_rate, max_delay_ms);
}

void FeedbackDelay::setDelay(float delay_ms) {
	delay_line.setDelay(delay_ms);
}

void FeedbackDelay::setFeedback(float feedback) {
	if (feedback < 0.0f || feedback >= 1.0f) {
		throw std::invalid_argument("Feedback must be between 0.0 and 1.0!");
	}
	feedback_coefficient = feedback;
}

void FeedbackDelay::setWetMix(float wm) {
	if (wm < 0.0f || wm > 1.0f) {
		throw std::invalid_argument("Wet Mix must be between 0.0 and 1.0!");
	}
	wet_mix = wm;
}

// y[n] = x[n] + g * y[n−D]
// y_output​[n]=(1−w) * x[n] + w * y_feedback​[n]
float FeedbackDelay::processSample(float input) {
	float delayed_output = delay_line.readDelayedSample();
	float new_input = input + (feedback_coefficient * delayed_output);
	delay_line.writeSample(new_input);
	return (((1 - wet_mix) * input) + (wet_mix * delayed_output));
}

void FeedbackDelay::reset() {
	delay_line.reset();
}