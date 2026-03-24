#pragma once
#include <vector>
#include "atae/types/audio_buffer.h"

class OnePoleLpf {
public:
	// called once before processing
	void setCutoff(double cutoff_frequency, int sample_rate, int channel_count);
	void apply(AudioBuffer& buffer);
	void reset();
private:
	double blendCoefficient = 0.0;
	double oneMinusBlendCoefficient = 1.0; // pre-compute 1 - blendCoefficient
	std::vector<double> previousOutputPerChannel;
};