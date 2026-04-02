#pragma once
#include "atae/delay/delay_line.h"

class FeedbackDelay {
public:
	void prepare(int sample_rate, float max_delay_ms);
	void setDelay(float delay_ms);
	void setFeedback(float feedback);
	void setWetMix(float wm);
	float processSample(float input);
	void reset();

private:
	DelayLine delay_line;
	float feedback_coefficient = 0.0f;
	float wet_mix = 0.0f;
};