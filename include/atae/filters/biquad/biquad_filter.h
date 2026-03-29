#pragma once
#include <vector>
#include "atae/types/audio_buffer.h"

enum class BiquadFilterType { LPF, HPF, LowShelf, HighShelf, PeakingEQ };

class BiquadFilter {
public:
	void prepare(int sr, int ch); // sample rate and channel count
	void setCoefficients(BiquadFilterType filter_type, double cutoff_frequency, double quality_factor, double db_gain);
	void apply(AudioBuffer& buffer);
	void reset();

private:
	std::vector<double> x1PerChannel; // x[n-1]
	std::vector<double> x2PerChannel; // x[n-2]
	std::vector<double> y1PerChannel; // y[n-1]
	std::vector<double> y2PerChannel; // y[n-2]

	double feedforward_b0 = 1;
	double feedforward_b1 = 0;
	double feedforward_b2 = 0;
	double feedback_a1 = 0;
	double feedback_a2 = 0;

	int sample_rate = 0;
	int channel_count = 0;
};