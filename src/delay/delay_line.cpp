#include <algorithm>
#include <cmath>
#include <ranges>
#include <stdexcept>
#include "atae/delay/delay_line.h"

void DelayLine::prepare(int sr, float max_delay_ms) {
	if (max_delay_ms < 0.0) {
		throw new std::invalid_argument("Max delay should be greater than 0.0!");
	}
	if (sr <= 0) {
		throw new std::invalid_argument("Sample rate must be greater than 0!");
	}

	write_index = 0;
	sample_rate = sr;

	max_delay_samples = max_delay_ms * (sr / 1000.0);
	if (max_delay_samples < 1) {
		throw new std::invalid_argument("Buffer size is less than 1! Please increase your sample rate or max_delay_ms!");
	}
	delay_buffer.resize(max_delay_samples);
}

void DelayLine::setDelay(float delay_ms) {
	if (delay_ms < 0.0 || delay_ms > (max_delay_samples * 1000.0 / sample_rate)) {
		throw new std::invalid_argument("Delay should be between 0.0 and max delay!");
	}

	delay_samples = delay_ms * (sample_rate / 1000.0);
}

float DelayLine::processSample(float input) {
	delay_buffer[write_index] = input;

	int read_index = write_index - delay_samples;
	if (read_index < 0) {
		read_index += static_cast<int>(delay_buffer.size());
	}

	float output = delay_buffer[read_index];

	write_index++;
	if (write_index >= static_cast<int>(delay_buffer.size())) {
		write_index = 0;
	}

	return output;
}

void DelayLine::reset() {
	write_index = 0;
	std::ranges::fill(delay_buffer, 0.0);
}