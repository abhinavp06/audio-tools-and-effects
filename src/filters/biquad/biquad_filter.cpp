#include <vector>
#include "atae/filters/biquad/biquad_filter.h"

void BiquadFilter::prepare(int sr, int ch) {
	sample_rate = sr;
	channel_count = ch;

	x1PerChannel.resize(ch);
	x2PerChannel.resize(ch);
	y1PerChannel.resize(ch);
	y2PerChannel.resize(ch);
}