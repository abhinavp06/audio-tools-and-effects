#include "atae/delay/delay_line.h"

class CombFilter {
public:
	void prepare(int sample_rate, float max_delay_ms);
	void setDelay(float delay);
	void setGain(float g);
	float processSample(float input);
private:
	DelayLine delay_line;
	float gain = 0.0f;
};