#include <algorithm>
#include <cmath>
#include <ranges>
#include <stdexcept>
#include "atae/delay/delay_line.h"

void DelayLine::prepare(int sr, float max_delay_ms) {
    if (max_delay_ms < 0.0f) {
        throw new std::invalid_argument("Max delay should be greater than 0.0!");
    }
    if (sr <= 0) {
        throw new std::invalid_argument("Sample rate must be greater than 0!");
    }

    write_index = 0;
    sample_rate = sr;

    max_delay_samples = max_delay_ms * (sr / 1000.0f);

    if (max_delay_samples < 1.0f) {
        throw new std::invalid_argument("Buffer size is less than 1! Please increase your sample rate or max_delay_ms!");
    }

    delay_buffer.resize(static_cast<size_t>(std::ceil(max_delay_samples)) + 1);
}

void DelayLine::setDelay(float delay_ms) {
    float max_delay_ms = max_delay_samples * 1000.0f / sample_rate;

    if (delay_ms < 0.0f || delay_ms > max_delay_ms) {
        throw new std::invalid_argument("Delay should be between 0.0 and max delay!");
    }

    delay_samples = delay_ms * (sample_rate / 1000.0f);
}

float DelayLine::processSample(float input) {
    delay_buffer[write_index] = input;

    float read_pos = static_cast<float>(write_index) - delay_samples;
    if (read_pos < 0.0f) {
        read_pos += static_cast<float>(delay_buffer.size());
    }

    int index0 = static_cast<int>(std::floor(read_pos));
    int index1 = index0 + 1;

    if (index1 >= static_cast<int>(delay_buffer.size())) {
        index1 = 0;
    }

    float frac = read_pos - static_cast<float>(index0);

    float sample0 = delay_buffer[index0];
    float sample1 = delay_buffer[index1];

    float output = (1.0f - frac) * sample0 + frac * sample1;

    write_index++;
    if (write_index >= static_cast<int>(delay_buffer.size())) {
        write_index = 0;
    }

    return output;
}

void DelayLine::reset() {
    write_index = 0;
    std::ranges::fill(delay_buffer, 0.0f);
}