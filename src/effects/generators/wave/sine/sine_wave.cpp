#include "sine_wave.h"
#include <random>

/*
* y(t) = A * sin(2π * f * t + φ)
* Where:
*	A - amplitude (volume, between 0.0, 1.0)
*	f - frequency in Hz
*	t - time in seconds
*	φ - phase offset in radians - controls where in the cycle we start
*
* dt = 1 / sample_rate => t = i * dt = i / sample_rate => sample[i] = A * sin(2π * f * i / sample_rate)
*
*/

AudioBuffer SineWave::generate_naive(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {

}

/*
* What happens when we generate a very long sine wave (maybe around 10 minutes at 44.1kHz)?
* -> The value of i gets very large which makes i/sample_rate very large
*	 Mathematically, it's fine since sine is periodic however float and double have their limitations
*	 As the number gets larger, the sine wave slowly starts drifting our of tune
* 
* To fix this, phase accumulation is used
* Instead of computing t from i every sample, a running phase variable is kept and incremented by a fixed amount each sample
*   phase_increment = 2π * f / sample_rate
*   phase = 0.0
*   each sample:
*       sample[i] = A * sin(phase)
*       phase += phase_increment
*       if phase >= 2π: phase -= 2π  ← keep phase in [0, 2π]
*
*/
AudioBuffer SineWave::generate(const int duration_s, const int sample_rate, const int channel_count, const double frequency, const double amplitude) {

}
