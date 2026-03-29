#include <algorithm>
#include <cassert>
#include <cmath>
#include <numbers>
#include <ranges>
#include <stdexcept>
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

void BiquadFilter::setCoefficients(BiquadFilterType filter_type, double cutoff_frequency, double quality_factor, double db_gain) {
	if (x1PerChannel.empty()) {
		throw std::runtime_error("BiquadFilter must be prepared before setCoefficients!");
	}
	
	if (cutoff_frequency <= 0.0 || cutoff_frequency >= sample_rate / 2.0) {
		throw std::invalid_argument("Cutoff must be > 0 and < Nyquist!");
	}

	double w0 = 2 * std::numbers::pi * (cutoff_frequency / sample_rate); // normalized angular frequency
	double alpha = std::sin(w0) / (2 * quality_factor);
	double A = 0.0; // linear gain factor
	double a0 = 0.0;
	if (filter_type == BiquadFilterType::LowShelf || filter_type == BiquadFilterType::HighShelf || filter_type == BiquadFilterType::PeakingEQ) {
		A = std::pow(10, (db_gain / 40));
	}

	switch (filter_type) {
	case BiquadFilterType::LPF:
		feedforward_b0 = (1 - std::cos(w0)) / 2;
		feedforward_b1 = 1 - std::cos(w0);
		feedforward_b2 = feedforward_b0;

		a0 = 1 + alpha;
		feedback_a1 = (-2 * std::cos(w0));
		feedback_a2 = (1 - alpha);
		break;
	case BiquadFilterType::HPF:
		feedforward_b0 = (1 + std::cos(w0)) / 2;
		feedforward_b1 = -1 * (1 + std::cos(w0));
		feedforward_b2 = feedforward_b0;

		a0 = 1 + alpha;
		feedback_a1 = (-2 * std::cos(w0));
		feedback_a2 = (1 - alpha);
		break;
	case BiquadFilterType::LowShelf:
		feedforward_b0 = A * ((A + 1) - ((A - 1) * std::cos(w0)) + (2 * std::sqrt(A) * alpha));
		feedforward_b1 = 2 * A * ((A - 1) - ((A + 1) * std::cos(w0)));
		feedforward_b2 = A * ((A + 1) - ((A - 1) * std::cos(w0)) - (2 * std::sqrt(A) * alpha));

		a0 = (A + 1) + ((A - 1) * std::cos(w0)) + (2 * std::sqrt(A) * alpha);
		feedback_a1 = -2 * ((A - 1) + ((A + 1) * std::cos(w0)));
		feedback_a2 = (A + 1) + ((A - 1) * std::cos(w0)) - (2 * std::sqrt(A) * alpha);
		break;
	case BiquadFilterType::HighShelf:
		feedforward_b0 = A * ((A + 1) + ((A - 1) * std::cos(w0)) + (2 * std::sqrt(A) * alpha));
		feedforward_b1 = -2 * A * ((A - 1) + ((A + 1) * std::cos(w0)));
		feedforward_b2 = A * ((A + 1) + ((A - 1) * std::cos(w0)) - (2 * std::sqrt(A) * alpha));

		a0 = (A + 1) - ((A - 1) * std::cos(w0)) + (2 * std::sqrt(A) * alpha);
		feedback_a1 = 2 * ((A - 1) - ((A + 1) * std::cos(w0)));
		feedback_a2 = (A + 1) - ((A - 1) * std::cos(w0)) - (2 * std::sqrt(A) * alpha);
		break;
	case BiquadFilterType::PeakingEQ:
		feedforward_b0 = 1 + (alpha * A);
		feedforward_b1 = -2 * std::cos(w0);
		feedforward_b2 = 1 - (alpha * A);

		a0 = 1 + (alpha / A);
		feedback_a1 = -2 * std::cos(w0);
		feedback_a2 = 1 - (alpha / A);
		break;
	}

	feedforward_b0 /= a0;
	feedforward_b1 /= a0;
	feedforward_b2 /= a0;
	feedback_a1 /= a0;
	feedback_a2 /= a0;
}

void BiquadFilter::apply(AudioBuffer& buffer) {
	assert(x1PerChannel.size() == buffer.channels);
	assert(sample_rate == buffer.sampleRate);

	for (int ch = 0; ch < buffer.channels; ch++) {
		for (size_t i = ch; i < buffer.samples.size(); i += buffer.channels) {
			double xn = buffer.samples[i];
			double yn = (feedforward_b0 * xn) + (feedforward_b1 * x1PerChannel[ch]) + (feedforward_b2 * x2PerChannel[ch]) - (feedback_a1 * y1PerChannel[ch]) - (feedback_a2 * y2PerChannel[ch]);

			buffer.samples[i] = yn;
			x2PerChannel[ch] = x1PerChannel[ch];
			x1PerChannel[ch] = xn;
			y2PerChannel[ch] = y1PerChannel[ch];
			y1PerChannel[ch] = yn;
		}

	}
}

void BiquadFilter::reset() {
	std::ranges::fill(x1PerChannel, 0.0);
	std::ranges::fill(x2PerChannel, 0.0);
	std::ranges::fill(y1PerChannel, 0.0);
	std::ranges::fill(y2PerChannel, 0.0);
}