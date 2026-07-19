#include <iostream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <vector>
#include "atae/filters/biquad/biquad_filter.h"
#include "atae/oscillators/square_wave.h"
#include "atae/oscillators/sine_wave.h"
#include "atae/noise/pink_noise.h"
#include "atae/mixer/mixer.h"
#include "atae/modulation/lfo.h"
#include <atae/io/audio_file.h>

struct AutowahParams {
    double lfo_rate_hz = 4.0;
    double lfo_depth = 1.0;
    LfoWaveform lfo_waveform = LfoWaveform::Sine;
    double f_center = 800.0;
    double f_depth = 600.0;
    double bf_qf = 0.707;
    double bf_db_gain = 6.0;
};

AudioBuffer makeSine(int duration_s, int sample_rate, int channel_count) {
    SineWave osc;
    return osc.generate(duration_s, sample_rate, channel_count, 440.0, 0.8);
}

AudioBuffer makeSquare(int duration_s, int sample_rate, int channel_count) {
    SquareWave osc;
    return osc.generate(duration_s, sample_rate, channel_count, 440.0, 0.8);
}

AudioBuffer makePinkNoise(int duration_s, int sample_rate, int channel_count) {
    return PinkNoise::generate(duration_s, sample_rate, channel_count, 0.8);
}

// a chord: root + major third + fifth, mixed together
AudioBuffer makeChord(int duration_s, int sample_rate, int channel_count) {
    SquareWave osc;
    AudioBuffer root = osc.generate(duration_s, sample_rate, channel_count, 220.0, 0.8); // A3
    AudioBuffer third = osc.generate(duration_s, sample_rate, channel_count, 277.18, 0.8); // C#4
    AudioBuffer fifth = osc.generate(duration_s, sample_rate, channel_count, 329.63, 0.8); // E4

    Mixer mixer;
    std::vector<MixInput> inputs = { {root, 0.5}, {third, 0.5}, {fifth, 0.5} };
    return mixer.mix(inputs, true);
}

void applyAutowah(AudioBuffer& buffer, const AutowahParams& p) {
    const int sample_rate = buffer.sampleRate;
    const int channel_count = buffer.channels;
    const double nyquist = sample_rate / 2.0;

    Lfo lfo;
    lfo.prepare(sample_rate);
    lfo.setRate(p.lfo_rate_hz);
    lfo.setDepth(p.lfo_depth);
    lfo.setWaveform(p.lfo_waveform);

    BiquadFilter bf;
    bf.prepare(sample_rate, channel_count);

    const int total_frames = static_cast<int>(buffer.samples.size()) / channel_count;
    for (int frame = 0; frame < total_frames; frame++) {
        double lfo_output = lfo.process();
        double cutoff_freq = std::clamp(p.f_center + p.f_depth * lfo_output, 20.0, nyquist - 1.0);
        bf.setCoefficients(BiquadFilterType::LPF, cutoff_freq, p.bf_qf, p.bf_db_gain);

        for (int ch = 0; ch < channel_count; ch++) {
            size_t index = (frame * channel_count) + ch;
            buffer.samples[index] = bf.processSample(buffer.samples[index], ch);
        }
    }
}

void runVariant(const std::string& name,
    AudioBuffer(*sourceBuilder)(int, int, int),
    int duration_s, int sample_rate, int channel_count,
    const AutowahParams& params) {
    std::cout << "Processing variant: " << name << std::endl;

    AudioBuffer buffer = sourceBuilder(duration_s, sample_rate, channel_count);
    AudioFile::save(OUTPUT_DIR + std::string("autowah_") + name + "_dry.wav", buffer);

    applyAutowah(buffer, params);
    AudioFile::save(OUTPUT_DIR + std::string("autowah_") + name + "_wet.wav", buffer);
}

int main() {
    std::cout << "----- AUTOWAH by abhinavp06 -----" << std::endl;

    int duration_s = 10, sample_rate = 44100, channel_count = 2;

    AutowahParams params;
    params.lfo_rate_hz = 4.0;
    params.lfo_waveform = LfoWaveform::Sine;
    params.f_center = 800.0;
    params.f_depth = 600.0;

    try {
        runVariant("sine", makeSine, duration_s, sample_rate, channel_count, params);
        runVariant("square", makeSquare, duration_s, sample_rate, channel_count, params);
        runVariant("pink", makePinkNoise, duration_s, sample_rate, channel_count, params);
        runVariant("chord", makeChord, duration_s, sample_rate, channel_count, params);
    }
    catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    std::cout << "----- GOODBYE -----" << std::endl;
    return 0;
}

/**
* My observations (wet signal):
* 1. SQUARE WAVE: 
*		- constant faint low sound
*		- like a bassline sound (not a bass.. just a faint high pitch sound)
*		- then I have a beep in some set interval (which i am assuming is the peak and trough of the square wave)
* 2. SINE WAVE:
*		- can get a sense of the "wah"
*		- "wah" will be more pronounced if we have multiple waves
*		a. SET LFO_RATE_HZ to 0.5:
*			- pretty much the same in terms of the sound
*			- the wah is so slow it barely reads as a wah
*			- however the modulation takes more time.. it's not as quick as it was with 1.0
*		b. SET LFO_RATE_HZ to 4.0:
*			- same as above.. just faster modulation
*			- it starts to feel rhythmic and vocal
*		c. SET LFO_RATE_HZ to 8.0:
*			- same as above.. even faster modulation
*			- it approaches a warble
* 
* MODULATION EFFECTS — SAME LFO, DIFFERENT TARGETS:
* They all share the same modulator — an LFO — but they route it to different
* targets. That's the entire distinction. The LFO is just a knob-turner;
* what changes is which knob it turns.
*
* 1. AUTOWAH:
*		- LFO drives filter cutoff
*		- sweeping which frequencies pass through
*		- the timbre/brightness changes (what was built here)
* 2. TREMOLO:
*		- LFO drives amplitude
*		- sweeping how loud the signal is
*		- volume goes up and down (think shimmer in surf-rock guitar)
* 3. VIBRATO:
*		- LFO drives pitch (the oscillator's frequency)
*		- sweeping how high or low the note is
*		- think of a singer wobbling a sustained note
*
* PEDAL POSITION:
*		- cutoff_freq is the pedal position at any instant
*		- LFO output high  -> cutoff high -> pedal down -> filter open -> bright
*		- LFO output low   -> cutoff low  -> pedal up   -> filter closed -> dark
*		- real pedal: potentiometer under a foot; autowah: the LFO is the foot
*/