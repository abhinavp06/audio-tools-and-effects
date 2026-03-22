#pragma once

struct Harmonic {
	double amplitude; // 1 / harmonic
	double phase_increment; // 2π * f * harmonic / sample_rate
	double phase; // current phase for this harmonic
};