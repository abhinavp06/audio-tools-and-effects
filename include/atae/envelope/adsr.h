#pragma once

enum class AdsrState { Idle, Attack, Decay, Sustain, Release };

class Adsr {
public:
	void setAttack(double attack_s, int sample_rate);
	void setSustain(double sustain_level);
	void setDecay(double decay_s, int sample_rate);
	void setRelease(double release_s, int sample_rate);
	void noteOn();
	void noteOff();
	double process();
	AdsrState getState() const;

private:
	AdsrState state = AdsrState::Idle;
	double currentValue = 0.0;
	double attackIncrement = 0.0;
	double decayIncrement = 0.0;
	double releaseIncrement = 0.0;
	int releaseSamples = 0;
	int decaySamples = 0;
	double sustainLevel = 0.0;
};