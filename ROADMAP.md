# OVERVIEW
## FOUNDATIONS
- White Noise
- Oscillators (Sine, Square, Saw)
- Mixer
- Pink Noise

## FILTERS
- One-Pole LPF (first order, one coefficient)
- Biquad Filter (second order)
   - Low Pass
   - High Pass
   - Low Shelf
   - High Shelf
   - Peaking EQ

## TIME DOMAIN
- Delay Line (ring buffer)
- Feedback Delay
- Comb Filter

## DYNAMICS
- Envelope Follower (RMS + Peak)
- Compressor
- Limiter

## ENVELOPES & MODULATION
 ADSR
- LFO (Low Frequency Oscillator)
- LFO - Filter Cutoff Modulation (autowah territory)

## SYNTHESIS
- Simple Subtractive Synth Voice (Oscillator + Filter + ADSR + Amp)
- Polyphonic Voice Manager

# NODE WISE ===>
- **Node 01** -> White Noise Generator  
- **Node 02** -> Sine Wave Oscillator  
- **Node 03** -> Square Wave Oscillator  
- **Node 04** -> Saw Wave Oscillator  
- **Node 05** -> Mixer  
- **Node 06** -> One-Pole Low Pass Filter  
- **Node 07** -> ADSR Envelope  
- **Node 08** -> Pink Noise Generator  

- **Node 09** -> Biquad Filter (LPF, HPF, Low Shelf, High Shelf, Peaking EQ)  

- **Node 10** -> Delay Line (Ring Buffer)  
- **Node 11** -> Feedback Delay  
- **Node 12** -> Comb Filter  

- **Node 13** -> Envelope Follower (RMS + Peak)  
- **Node 14** -> Compressor  
- **Node 15** -> Limiter  

- **Node 16** -> LFO  
- **Node 17** -> LFO - Filter Cutoff Modulation  

- **Node 18** -> Subtractive Synth Voice (Oscillator + Filter + ADSR + Amp)  
- **Node 19** -> Polyphonic Voice Manager  

# POST-NODE ROADMAP (After Node 19)

- Building professional DSP effects
- Creating VST3/AU plugins (Ableton, FL Studio, etc.)
- Developing tools useful for IEM tuning
- Eventually reaching audio codec development

---

# PHASE 2: DSP ENGINE FOUNDATION (Must-Have Core)

- **Node 20** -> Audio Buffer Utilities + Channel Routing  
  (interleaved/non-interleaved conversion, downmix/upmix, stereo copy utilities)

- **Node 21** -> Parameter Smoothing / De-Zippering  
  (1-pole smoothing, exponential smoothing for automation-safe knobs)

- **Node 22** -> Audio Metering (Peak, RMS, dBFS)  
  (basic meters required for plugins)

- **Node 23** -> DC Blocker Filter  
  (removes DC offset, required for distortion/saturation chains)

- **Node 24** -> Sample Rate Conversion (SRC) - Linear Interpolation  
  (basic resampling foundation)

- **Node 25** -> Sample Rate Conversion (SRC) - Windowed Sinc Resampler  
  (high quality resampler, important for plugins + codec groundwork)

- **Node 26** -> Oversampling Engine (2x / 4x / 8x)  
  (upsample -> lowpass -> downsample)

---

# PHASE 3: ADVANCED TIME EFFECTS (Plugin-Grade)

- **Node 27** -> Allpass Filter  
  (core building block for diffusion/reverb/phaser)

- **Node 28** -> Feedback Delay Network (FDN) Basics  
  (foundation of modern reverbs)

- **Node 29** -> Chorus Effect (LFO Modulated Delay)

- **Node 30** -> Flanger Effect (Short Delay + Feedback + LFO)

- **Node 31** -> Phaser Effect (Allpass Filter Chain + LFO)

- **Node 32** -> Stereo Delay (Ping Pong Delay)  
  (stereo routing + feedback design)

- **Node 33** -> Schroeder Reverb  
  (comb filters + allpass diffusion)

- **Node 34** -> Plate Reverb (Simplified Model)  
  (more dense diffusion-based reverb)

---

# PHASE 4: DISTORTION / SATURATION (Essential for Modern Plugins)

- **Node 35** -> Hard Clip / Soft Clip / Tanh Waveshaping Distortion

- **Node 36** -> Distortion with Tone Filter (Pre/Post EQ)

- **Node 37** -> Oversampled Distortion  
  (use Node 26 oversampling to reduce aliasing)

- **Node 38** -> Bitcrusher (Bit Depth + Sample Rate Reduction)

- **Node 39** -> Noise Gate

---

# PHASE 5: FFT + SPECTRAL PROCESSING (Codec & Analyzer Foundation)

- **Node 40** -> FFT Wrapper (FFTW / KissFFT integration)

- **Node 41** -> Window Functions (Hann, Hamming, Blackman)

- **Node 42** -> Spectrum Analyzer (Magnitude Spectrum Plot Data)

- **Node 43** -> Spectrogram Generator (Time-Frequency Display)

- **Node 44** -> FIR Filter Designer (Lowpass / Highpass / Bandpass)

- **Node 45** -> Convolution (FIR Processing)

- **Node 46** -> Convolution Reverb (Impulse Response Based)

- **Node 47** -> Partitioned Convolution (Fast Convolution Reverb)

---

# PHASE 6: PITCH + TIME PROCESSING (Advanced DSP Skill)

- **Node 48** -> Zero Crossing Pitch Detection (Basic)

- **Node 49** -> Autocorrelation Pitch Detection (Better)

- **Node 50** -> Phase Vocoder (Time Stretch Basics)

- **Node 51** -> Pitch Shifter (Phase Vocoder Based)

---

# PHASE 7: SYNTH ADVANCEMENT (Towards Full Plugin Synths)

- **Node 52** -> Wavetable Oscillator

- **Node 53** -> Unison Engine (Detune + Stereo Spread)

- **Node 54** -> Modulation Matrix (LFO/Env routing system)

- **Node 55** -> Mono/Poly Voice Stealing System (Professional Polyphony)

- **Node 56** -> Step Sequencer / Arpeggiator

---

# PHASE 8: PLUGIN DEVELOPMENT TRACK (JUCE / VST3 / AU)

> This is where you begin building actual plugins usable in Ableton/FL Studio.

- **Node 57** -> JUCE Setup + Audio Callback Basics

- **Node 58** -> Plugin Parameters + State Saving (Presets)

- **Node 59** -> Plugin UI Basics (Knobs, Sliders, Layout)

- **Node 60** -> Plugin 01: Gain + Meter Plugin  
  (your first shippable plugin)

- **Node 61** -> Plugin 02: Parametric EQ Plugin  
  (your biquad filter becomes a real EQ)

- **Node 62** -> Plugin 03: Compressor Plugin  
  (attack/release/ratio/knee + metering)

- **Node 63** -> Plugin 04: Delay Plugin  
  (feedback delay + ping pong + dry/wet)

- **Node 64** -> Plugin 05: Reverb Plugin  
  (Schroeder/FDN style)

- **Node 65** -> Plugin 06: Saturation / Distortion Plugin  
  (oversampling + waveshaper)

---

# PHASE 9: IEM / AUDIO MEASUREMENT TOOLING TRACK

> This phase is focused on building tools that will help you tune your own IEMs.

- **Node 66** -> Sweep Generator + Impulse Response Capture Tools

- **Node 67** -> Frequency Response Estimation Tool  
  (FFT analysis of sweep response)

- **Node 68** -> Target Curve Matching EQ Generator  
  (auto-generate parametric EQ settings)

- **Node 69** -> Loudness Matching / Perceptual Volume Matching Tools

- **Node 70** -> Stereo Imaging Tools (Width, Mid/Side)

---

# PHASE 10: AUDIO CODEC TRACK

> This is the long-term track for codec design.

- **Node 71** -> MDCT Implementation (Transform used in AAC/MP3)

- **Node 72** -> Critical Bands / Bark Scale Implementation

- **Node 73** -> Psychoacoustic Masking Model (Basic)

- **Node 74** -> Quantization + Noise Shaping

- **Node 75** -> Bit Allocation System

- **Node 76** -> Entropy Coding (Huffman / Arithmetic)

- **Node 77** -> Bitstream Framing + Packetization

- **Node 78** -> Build a Simple Lossy Codec Prototype  
  (encode + decode pipeline)

- **Node 79** -> Compare Quality vs Bitrate (Test Bench)

---

# PHASE 11: FINAL PROFESSIONAL POLISH

- **Node 80** -> Denormal Protection + CPU Optimization

- **Node 81** -> SIMD Optimization (SSE/AVX Basics)

- **Node 82** -> Multithreaded Audio Processing (Careful + Lock-Free)

- **Node 83** -> Real-Time Safe Memory Management  
  (no allocations in audio callback)

- **Node 84** -> Full Plugin Release Pipeline  
  (installer, versioning, presets, docs)

---