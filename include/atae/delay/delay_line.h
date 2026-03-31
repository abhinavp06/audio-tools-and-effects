#pragma once
#include <vector>

class DelayLine {
public:
    void prepare(int sr, float max_delay_ms);
    void setDelay(float delay_ms);
    float processSample(float input);
    void reset();
private:
    float delay_samples = 0.0f;
    float max_delay_samples = 0.0f;
    float max_delay_ms = 0.0f;
    int sample_rate = 0;
    int write_index = 0;
    int buffer_mask = 0;
    std::vector<float> delay_buffer;
};