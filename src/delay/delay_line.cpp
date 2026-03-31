#include <algorithm>
#include <cmath>
#include <ranges>
#include <stdexcept>
#include "atae/delay/delay_line.h"

static int nextPowerOfTwo(int n) {
    int power = 1;
    while (power < n) {
        power <<= 1;
    }
    return power;
}

void DelayLine::prepare(int sr, float max_delay_ms_value) {
    if (max_delay_ms_value < 0.0f) {
        throw std::invalid_argument("Max delay should be greater than 0.0!");
    }
    if (sr <= 0) {
        throw std::invalid_argument("Sample rate must be greater than 0!");
    }

    write_index = 0;
    sample_rate = sr;

    max_delay_ms = max_delay_ms_value;
    max_delay_samples = max_delay_ms * (sample_rate / 1000.0f);

    if (max_delay_samples < 1.0f) {
        throw std::invalid_argument("Buffer size is less than 1! Please increase your sample rate or max_delay_ms!");
    }

    int required_size = static_cast<int>(std::ceil(max_delay_samples)) + 1;
    int pow2_size = nextPowerOfTwo(required_size);

    delay_buffer.resize(pow2_size);
    buffer_mask = pow2_size - 1;
}

void DelayLine::setDelay(float delay_ms) {
    if (delay_ms < 0.0f || delay_ms > max_delay_ms) {
        throw std::invalid_argument("Delay should be between 0.0 and max delay!");
    }

    delay_samples = delay_ms * (sample_rate / 1000.0f);
}

float DelayLine::processSample(float input) {
    delay_buffer[write_index] = input;

    float read_pos = static_cast<float>(write_index) - delay_samples;
    if (read_pos < 0.0f) {
        read_pos += static_cast<float>(delay_buffer.size());
    }

    float base = std::floor(read_pos);

    int index0 = static_cast<int>(base) & buffer_mask;
    int index1 = (index0 + 1) & buffer_mask;

    float frac = read_pos - base;

    float sample0 = delay_buffer[index0];
    float sample1 = delay_buffer[index1];

    float output = (1.0f - frac) * sample0 + frac * sample1;

    write_index = (write_index + 1) & buffer_mask;

    return output;
}

void DelayLine::reset() {
    write_index = 0;
    std::ranges::fill(delay_buffer, 0.0f);
}