#include "atae/envelope/adsr.h"

void Adsr::setAttack(double attack_s, int sample_rate) {
    if (attack_s <= 0.0) {
        attackIncrement = 1.0; // instant jump
    }
    else {
        attackIncrement = 1.0 / (attack_s * sample_rate);
    }
}

void Adsr::setSustain(double sustain_level) {
    sustainLevel = sustain_level;
}

void Adsr::setDecay(double decay_s, int sample_rate) {
    decaySamples = decay_s * sample_rate;
}

void Adsr::setRelease(double release_s, int sample_rate) {
    releaseSamples = release_s * sample_rate;
}

void Adsr::noteOn() {
    currentValue = 0.0;
	state = AdsrState::Attack;
}

void Adsr::noteOff() {
    state = AdsrState::Release;
    releaseIncrement = (releaseSamples > 0) ? currentValue / releaseSamples : currentValue;
}

double Adsr::process() {
    switch (getState()) {
    case AdsrState::Idle:
        return 0.0;
    case AdsrState::Attack:
        if (currentValue >= 1.0) {
            state = AdsrState::Decay;
            currentValue = 1.0;
            if (decaySamples <= 0) {
                decayIncrement = 1.0;
            }
            else {
                decayIncrement = (1.0 - sustainLevel) / decaySamples;
            }
        }
        else {
            currentValue += attackIncrement;
        }
        break;
    case AdsrState::Decay:
        if (currentValue <= sustainLevel) {
            currentValue = sustainLevel;
            state = AdsrState::Sustain;
        }
        else {
            currentValue -= decayIncrement;
        }
        break;
    case AdsrState::Sustain:
        currentValue = sustainLevel;
        return sustainLevel;
    case AdsrState::Release:
        if (currentValue <= 0.0) {
            currentValue = 0.0;
            state = AdsrState::Idle;
        }
        else {
            currentValue -= releaseIncrement;
        }
        break;
    }

    return currentValue;

}

AdsrState Adsr::getState() const {
	return state;
}