#include <stdexcept>
#include "atae/delay/delay_line.h"
#include "atae/filters/comb/comb_filter.h"

void CombFilter::prepare(int sample_rate, float max_delay_ms) {
	delay_line.prepare(sample_rate, max_delay_ms);
}

void CombFilter::setDelay(float delay_ms) {
	delay_line.setDelay(delay_ms);
}

void CombFilter::setGain(float g) {
	if (g < 0.0 || g >= 1) {
		throw std::invalid_argument("Gain must be between 0.0 and 1.0!");
	}
	gain = g;
}

float CombFilter::processSample(float input) {
	float delayed_output = delay_line.readDelayedSample();
	float output = input - (gain * delayed_output);
	delay_line.writeSample(output);
	return output;
}
