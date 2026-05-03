## 1. In your own words: what is white noise, and why is the word "white" used to describe it?
White noise is a type of soundwave which contains equal energy throughout all frequencies (power per unit bandwidth). It is called "white" since it takes the whole spectrum into account.

## 2. Why use std::mt19937 instead of std::rand() in the white noise implementation?
- The C++ standard does not specify which algorithm std::rand() uses. Two different compilers can produce completely different sequences for the same seed. The noise would sound different on Windows vs Linux.
- Most std::rand() implementations use a simple Linear Congruential Generator (LCG). LCGs have known patterns which lead to predictable structures.
- std::rand() uses a global state which can cause issues in multithreaded programming.
- The range of std::mt19937 is higher compared to std::rand (2^32 - 1 vs 2^31). This gives much more randomness.

## 3. What is the difference between generate_naive() & generate() functions in the sine wave implementation?
In generate_naive(), for a very long duration, the value of i gets very large which makes i/sample_rate very large. Mathematically, it's fine since sine is periodic however float and double have their limitations. As the number gets larger, the sine wave slowly starts drifting our of tune.
The generate() function introduces a running phase variable which resets after every cycle (2 * pi).

## 4. What is channel-interleaving? Give a pictorial representation of a stereo sample.
Channel interleaving is the combination of information of multiple channels in a single buffer.
```
L0  R0  L1  R1  L2  R2  L3  R3  ....
|___|   |___|   |___|   |___|
 F1       F2      F3      F4

For a stereo input, the left and right channel are interleaved in a single buffer. Every 2 indexes is a "frame".
```

## 5. What is aliasing, and why does the naive square wave suffer from it but the naive sine wave does not?
Aliasing is a phenomenon which occurs when frequencies fold over (when they are above the Nyquist Limit).
A sine wave doesn't suffer from it since it is a single frequency. A square wave is built from infinite odd harmonics. Therefore, the frequencies in the square wave have a possibility of folding over if they execeed the Nyquist Limit.

## 6.In the square wave implementation, the generate() function normalizes the buffer after summing all the harmonics. Why? What would happen if the normalization is skipped?
Since we sum the harmonics, they may cross the boundaries of -1.0 and 1.0. This is why we normalize the values. If normalization is skipped, there would be clipping.

## 7. A saw wave and a square wave are both built from harmonic series. What is the key difference between the two series — which harmonics are included, and how do the amplitudes differ?
A square wave is the sum of all odd harmonics whereas a saw wave contains all harmonics with an alternating phase. Square wave is 2x stronger than the saw wave for the same  peak amplitude. A saw wave is brighter and harsher than a square wave.

## 8. In mixer.cpp, the mix() function accepts a normalize flag. What is the practical difference between calling mix() with normalize = true vs normalize = false? Give a real scenario where one would want each.
When we mix multiple audio buffers, it is possible that the final output may exceed the -1.0 to 1.0 range. By normalizing, we can contain the output. If we are creating a simple mix of certain buffers and only want the output of their mix, normalizing makes sense. However, if I am operating with multiple mixes, normalization can come later to preserve the "real energy" of each mix and produce the perfect blend. 

(Claude: One small correction: "preserve the real energy" is the right instinct, but technically normalization doesn't destroy energy relationships between samples within a buffer — it scales everything uniformly. What it destroys is the absolute level relationship between buffers in a chain. Small distinction, worth being precise about.)

## 9. What does a low pass filter actually do to a signal — describe it as a physical/visual phenomenon, not just "removes high frequencies."
If there is a frequency distribution graph (a curved line on a simple x-y plot), adding a low pass filter should add a curve to the right side of the graph and point it downwards.

Claude: You've described what it looks like on a frequency response plot. Good. Now tell me what that sounds like and what it looks like in the time domain — on a waveform, not a spectrum.
Specifically: if you feed a perfect square wave through a low pass filter with a low cutoff frequency, what does the output waveform look like compared to the input?

In the waveform, if I "cut off" high frequencies, ideally, the sharp edges of the square wave should "smooth out".

## 10. Look at your one_pole_lpf.cpp. The core filter equation is:
	y[n] = α * x[n] + (1 − α) * y[n−1]
#### What does each term represent physically? And what happens to the filter's behavior as α\alpha α approaches 0 vs as alpha approaches 1?
The y is the output, x is the input, n is the current sample, alpha is the blend coefficient.

When alpha approaches 0, the output closely resembles the previous output (which if traced back should be equal to ~0). When alpha approaches 1, the output is ~equal to the input.

## 11. Describe the four stages of an ADSR envelope in terms of what they do to the amplitude of a signal over time. Use a piano key press as your analogy.
Let's say there is a "state" which represents the lifecycle of a piano key press. Initially, the state is "Idle". When the key is first pressed, it changes to "Attack". It stays in attack until it reaches it's peak (1.0) as it increments gradually (attackIncrement). Then it moves to the "Decay" state. As it transitions to this decay state, we check if the user has provided a decaySamples input. If they have, then we gradually start chipping away the "power" of the sound. If they haven't, then we immediately decay the sound to the sustain level. As it is gradually decaying, it moves to the sustain mode when the desired sustain value is reached (input taken from user). Then we manually turn the note "off" and it transitions to a "Release" state and we gradually move towards 0 sound.

## 12. White noise has flat power spectral density. Pink noise does not. What is the difference in how pink noise's energy is distributed across frequencies, and why is pink noise considered more "natural" sounding than white noise?
In white noise, every frequency has the same "power". In pink noise, the power is distributed based on a set of frequencies. So, lower frequencies overall get more power compared to the higher frequencies. This is done to achieve a balanced output. 

The reason why pink noise is more natural is because the harshness of the high frequencies is decreased due to the power distribution.

## 13. Look at your pink_noise.cpp. You use 16 rows and a counter n with bit manipulation to decide which rows to update. What is the conceptual purpose of having multiple rows — why not just filter white noise directly?
Claude:
```
The row approach is O(log N) per sample — you only update the rows whose bit changed, which averages out to a very small number of updates per sample. A proper 1f\frac{1}{f}
f1​ filter applied to white noise would require a much more complex filter with carefully tuned coefficients — expensive and harder to get right.
And your second point is sharp: a simple LPF on white noise doesn't give you true 1f\frac{1}{f}
f1​ distribution. It just rolls off the top end crudely. The row-based Voss-McCartney algorithm produces the correct statistical distribution by construction — each octave band gets its own independent random contribution.
```

## 14. You implemented five biquad filter types. Without looking at the code — what is the conceptual difference between a Low Shelf and a Low Pass filter? They both affect low frequencies, so why are they different tools?
Low pass: Completely cutoff frequencies above a certain frequency
Low shelf: Boost or cut frequencies below a corner frequency

## 15. Your biquad filter stores x1, x2, y1, y2 per channel. What do these four variables represent, and why does resetting them to zero matter when you switch between filter types in your test?
history per channel
switching to zero matters because you don't want another filter's history to be passed on to the new filter

## 16. Your delay line uses a ring buffer with a power-of-two size and a bitmask. Why power-of-two specifically? What does the bitmask replace, and why is that replacement worth doing?
When write_index reaches the end, it needs to wrap back to 0. By using power-of-two, we are able to avoid using % for calculations.

Claude:
```
Modulo % on integers compiles down to a division instruction. Division is one of the most expensive CPU operations — it can take 20-40 clock cycles. In an audio callback processing 44100 samples per second, potentially across many voices and effect chains, you're calling this thousands of times per second.
With a power-of-two buffer size, the bitmask replaces it:
cppwrite_index = (write_index + 1) & buffer_mask;
A bitwise AND is a single clock cycle. It works because for any power-of-two size N, x % N is identical to x & (N-1). That's pure integer math — buffer_mask = pow2_size - 1 gives you all 1-bits below the size boundary, which naturally wraps the index.
This is your first taste of real-time safe, CPU-conscious DSP code. The habit of replacing modulo with bitmasks in hot loops is something you'll carry all the way to Node 84.
Score: 7/10. Needed the nudge but landed correctly.
```

## 17. Your readDelayedSample() uses linear interpolation between two samples. Why is interpolation needed at all — why can't you just read the sample at the exact delay position?
Interpolation is needed whenever the delay is not an integer number of samples (or when delay is changing). Without it, fractional delay is impossible and modulated delay effects sound bad.

## 18. Your feedback delay equation is:
```
y[n]=x[n]+g⋅y[n−D]y[n] = x[n] + g \cdot y[n-D]y[n]=x[n]+g⋅y[n−D]
```
## What happens physically when g (feedback coefficient) gets close to 1.0? And why did you enforce g < 1.0 strictly — why not g <= 1.0?

g < 1.0 strictly — even g = 1.0 is not safe to allow. The signal would eventually blow up into a wall of clipping noise. In a live audio context that means a loud, uncontrolled screech through speakers. Potentially damaging to equipment and ears.

## 19. A comb filter gets its name from what its frequency response looks like. Describe that shape — what does the frequency response plot look like, and why does it create that pattern?
A comb filter adds a delayed version of a signal to itself. This causes the waveform to look like a comb.

It creates this pattern because when a delayed sample is added back to the original signal, certain frequencies get cutoff and other frequencies are added to the original - depending on the phase and type of the comb filter.

## 20. You've now built a one-pole LPF, a biquad filter, and a comb filter. All three are filters. What is the fundamental architectural difference between a comb filter and the other two — in terms of how far back in history each one looks?
one-pole LPF -> 1 previous sample and output
biquad filter -> 2 previous samples and outputs
comb filter -> current input and the delayed output

A biquad and one pole LPF can only look into previous 1 or 2 samples.. this means that they can only shape something very "local"

In the case of the comb filter, I can shape the waveform on a bigger scale

## 21. Across your entire codebase, you process audio per-sample in a loop. In a real DAW plugin, audio arrives in blocks — typically 128 or 256 samples at a time. What problems could arise if you tried to drop your current implementations directly into a real-time block-processing context?

Heap management

## 22. Your Mixer::mix() has a bug that could cause a divide-by-zero. Can you find it?
max_sample_abs_value

if we are processing a sample which is just silence (pure silence) then it wil cause a division by 0

max_sample_abs_value is used to normalize the value to fit the scale of -1 to 1

## 23. You've built both a FeedbackDelay and a CombFilter. Looking at your implementations — what is the structural difference between the two? They both use a DelayLine internally.
In FeedbackDelay — the feedback path mixes the delayed signal back into the input before writing. So the delay buffer accumulates echoes of echoes — each repeat is a full, mixed signal. That's what gives you the classic decaying echo trail.

In CombFilter — the output (which already has the cancellation/addition baked in) is what gets written back. So the delay buffer stores the already-filtered signal. Each new sample is compared against a version of the signal that has already been through the comb processing. This creates the resonant, periodic interference pattern — the comb teeth — because the filter is recursively operating on its own output.

## 24. Your pink noise generator, your bandlimited square wave, and your biquad filter all deal with frequency content in different ways. In one sentence each — what is each one doing to the frequency spectrum, and what is the common underlying principle connecting all three?
Pink noise - Distributing energy based on octaves.. lower frequencies get more intense and higher frequencies less
Square wave - Deals with harmonics.. they are preiodic in nature
Biquad filter - A filter that helps certain frequencies pass and others not depending on it's type

The common principle is: every sound is a sum of sine waves at different frequencies, and all of these are just different ways of controlling the amplitude of those sine waves.

## 25. In adsr.cpp, when noteOff() is called, you compute releaseIncrement as currentValue / releaseSamples. Why currentValue specifically — why not always start the release from 1.0?
because there is a decay and sustain event that has been happening

Claude:
```
Correct. Expand it one step further — what specific problem would you hear if you always released from 1.0?
```
it would instantly jump from the sustain stage to 1.0

## 26. Look at your saw_wave.cpp generate_naive(). There's a subtle DC offset issue. Can you find it and explain why it produces a DC offset?
The phase reset condition.. when phase wraps around, it may be slightly above 2pi

## 27. Your delay_line.cpp allocates a buffer larger than strictly necessary — it rounds up to the next power of two. At 44100Hz with a max delay of 1000ms, how many samples does that require, and what is the actual allocated size?
It requires 44101 samples but the actual alocated sie is 65536 (since nextPowerOfTwo returns the smallest power of 2 which is >= 44101)

## 28. You've implemented both a FeedbackDelay and a CombFilter. A reverb algorithm (which you'll build at Node 33) uses both of these as building blocks. Based on what you know about how each one sounds — what role do you think each one plays in creating the perception of a reverberant space?
The feedback delay will act like the "base" and the comb filter can help amplify the effect

Claude:
```
Interesting instinct but "amplify the effect" is too vague. Let me give you a physical anchor.
When sound bounces around a room, two things happen:

You hear distinct early reflections — specific echoes from walls, floor, ceiling arriving at slightly different times
Those reflections bounce off each other repeatedly, creating a dense wash of sound that gradually decays

Which of those two phenomena maps to a feedback delay, and which maps to a comb filter?
```

```
A feedback delay produces discrete repeating echoes that decay over time — each repeat is a distinct copy of the signal. That maps directly to early reflections — specific, identifiable bounces off surfaces at different delay times.
A comb filter's periodically spaced interference pattern creates resonances at specific frequencies. When you network many comb filters together with different delay times, their overlapping resonances create a dense, diffuse wash — the reverb tail. No single echo is identifiable anymore, just a smooth decay.
So:

Feedback delay → early reflections
Comb filters (networked) → diffuse reverb tail

This is exactly the Schroeder reverb architecture at Node 33 — parallel comb filters feeding into allpass diffusers, creating a convincing room simulation from pure arithmetic.
```

## 29. Look at your one_pole_lpf.cpp apply() function. It iterates channel by channel using the stride pattern. Your biquad_filter.cpp apply() does the same. But your mixer.cpp iterates linearly — sample by sample without channel separation. Why is it safe for the mixer to ignore channel separation, but unsafe for the filters?
because in the mixer, we are simply adding multiple buffers and not actually processing any samples

## 30.If someone handed you a WAV file of an unknown signal and asked you to identify whether it was white noise, pink noise, a sine wave, a square wave, or a saw wave — without listening to it — what measurements or analyses would you perform on it, and what would you look for in each case?
First I would try to find out if the signal is periodic (maybe by taking sample rate and frequency into account). This would help me identify if it's one of the basic 3 waves - sine, square, saw or if it's noise

Claude:
```
Correct. Good first filter. Keep going.
You've split the problem into two branches: periodic vs non-periodic. Now tell me — once you've determined it's noise, how do you distinguish white from pink? And once you've determined it's periodic, how do you distinguish sine vs square vs saw?
```

White vs Pink:
try to find out the amplitudes.. if the lower frequencies have higher amplitude compared to the higher ones, then it's pink otherwise white

Sine vs Square vs Saw:
Try to find the sample with the highest amplitude.. if the very next sample after that is in the other phase, then it's either square or saw. Then check the next sample after that and see if it has the same value. If yes, it's a square wave otherwise saw.
If the very next sample after the peak is not in the other phase, then it's a sine wave

Claude:
```
White vs Pink — correct approach. The formal term for what you're describing is analyzing the power spectral density. Good instinct.
Sine vs Square vs Saw — you're thinking in the time domain, which works but is fragile. What if the frequency is very low and samples are far apart? Your "very next sample" approach breaks down.
There's a more robust approach that works regardless of frequency. You already know what each wave's harmonic content looks like:

Sine → one frequency only
Square → odd harmonics only, amplitudes 1/n
Saw → all harmonics, amplitudes 1/n

What tool would let you see the harmonic content of a signal directly?
```

```
a Fourier Transform (specifically an FFT) decomposes the signal into its frequency components. You'd look at the magnitude spectrum:

One spike at the fundamental, nothing else → sine wave
Spikes at fundamental + odd multiples only (f, 3f, 5f, 7f...) → square wave
Spikes at fundamental + all multiples (f, 2f, 3f, 4f, 5f...) → saw wave
Flat spectrum across all frequencies → white noise
Spectrum that slopes down 3dB per octave → pink noise

This is why the FFT is the single most important tool in all of DSP. Every identification, diagnosis, and verification problem reduces to "show me the frequency content." 
```

## 31. Your one-pole LPF coefficient is computed as:
## α=1−e−ωc\alpha = 1 - e^{-\omega_c}α=1−e−ωc​
## Why the exponential function specifically? What would happen if you used a simpler linear mapping — say 
## α=ωc/π\alpha = \omega_c / \pi α=ωc​/π — instead?
Human perception is logarithmic. A linear mapping would do the opposite. So in the low frequency region — small changes in α\alpha
α would cause large perceived jumps. In the high frequency region — large changes in α\alpha
α would cause barely perceptible movement.

## 32. Your biquad filter has a quality_factor parameter (Q). In your test you use Q = 0.707 for LPF/HPF. Why that specific value — what does Q = 0.707 mean for the filter's frequency response?
Q < 0.707 → gentle, sluggish rolloff, slight dip at cutoff
Q = 0.707 → maximally flat, smooth rolloff, no resonance
Q > 0.707 → resonant peak at cutoff, sharper rolloff after

## 33. Your CombFilter::setGain() enforces g >= 0.0. But mathematically, what would a negative gain value do to the comb filter's frequency response — which frequencies would be boosted and which would be cancelled?
out of phase signals would be flipped and added and the in phase ones would be flipped and removed... this will also mean that the graph will look like an upside down comb

## 34. Look at your pink_noise.cpp. You generate each channel independently in separate outer loops. Is this correct behavior for a stereo pink noise generator — should the left and right channels be correlated or independent, and why?
independent to maintain a stereo-like sound.. if they were the same then it would be a mono sound

## 35. You've built an ADSR and a Mixer. In your adsr_test.cpp, you apply the envelope by multiplying each sample by adsr.process(). If you wanted to apply the ADSR to control the cutoff frequency of your one-pole LPF instead of the amplitude — what would you need to change in your approach, and what problem would you immediately run into?
Thinking out loud:
```
thinking out loud:
So, I have a cutoff frequency.. I call noteOn() and it slowly starts ramping up to the cutoff frequency.. reaches a peak then decays, sustains and finally releasaes on noteoff(). That's the ADSR part

Now, in real-time, when the audio sample is being run through the One Pole LPF, the one pole LPF itself needs to filter stuff out.. so basically, according to the bucket analogy I would be manipulating the size of the "hole" in the bucket.. graudally increasing it, reachign the desired value and then closing it again.. this means that initially, I would not be filtering all the high frequencies required.. so it's like a 1->desired->1 pattern

this means that my cutoff frequency in a way is noit constant.. this would mean that the blend coefficient would initially be 0 then ramp up.. this means that there would be silence in the beginning?
```

Problems:
resize() — heap allocation every sample.

std::fill(..., 0.0) — resets the filter history to zero every single sample. The filter would never accumulate any history. It would produce silence or garbage.

## 36. Your delay_line.cpp uses float internally, but your AudioBuffer::samples is std::vector<double>. There's an implicit precision conversion happening. Where does it occur, and is it a problem?
It happens when I'm adding the values to the delayed sample.. it's a problem because I will lose precision

## 37. Your square_wave.cpp generate() computes max_harmonic like this:
```
int max_harmonic = static_cast<int>(nyquist_limit / frequency);
if (max_harmonic % 2 == 1) {
    max_harmonic = max_harmonic / 2 + 1;
} else {
    max_harmonic = max_harmonic / 2;
}

Walk me through what this logic is doing and why — what does max_harmonic represent before and after this block?
```
max_harmonic represents the max number of harmonics we need to add before aliasing starts to occur

before the if-block, max harmonic number
after the if-block gives the actual count of odd harmonics

## 38. Across your entire codebase, you use std::fmod to wrap phase values. Why std::fmod instead of a simple if-check like:
```
if (phase >= 2 * std::numbers::pi) phase -= 2 * std::numbers::pi;

Under what condition does the if-check fail but std::fmod handles correctly?
```
for the harmonic phases in the saw/square generators, high harmonic numbers can produce large increments — and fmod is the safe, correct tool.

## 39. Your Mixer validates that all input buffers have the same samples.size(). But is samples.size() the right thing to check? Could two buffers have the same samples.size() but still be incompatible for mixing?
Yes, if the channel count is different or the sample rate is different

## 40. Your entire codebase processes audio as double precision internally. Professional audio hardware operates at 32-bit float. Your AudioFile::save() converts double→float before writing. Is any information lost in that conversion, and does it matter for audio quality?
For listening not, since the difference is negligible. For processing, yes (especially long chains)

## 41. Your OnePoleLpf stores previousOutputPerChannel as a std::vector<double>. Your BiquadFilter stores x1PerChannel, x2PerChannel etc. also as vectors. These are the filter's memory — its state. If you were building a plugin that saved and loaded presets, would you need to save this filter state? Why or why not?
No, because those represent the "memory"...they are not a property of the filter

## 42. Your FeedbackDelay::processSample() reads the delayed sample first, then writes the new input. The order matters. What would happen — both mathematically and audibly — if you swapped the order: wrote first, then read?
Claude:
```
You write to write_index, then advance it, then read from (write_index - delay_samples) & mask. If the write just happened at the current position, and delay_samples is, say, 44100 — you're still reading from 44100 samples back.
The delay is preserved.
The actual problem is more subtle. You'd be reading the sample one position after where you intended — because the write_index advanced before the read. The effective delay becomes D - 1 samples instead of D samples.
For large delays that's inaudible — one sample off from 44100 is negligible. But for very short delays — like in a comb filter with a 10-sample delay — being off by one sample means being off by 10% of the delay time. That shifts all the comb teeth in frequency. Audible.
And for D = 0 (zero delay), you'd read the sample you just wrote — feeding the current input directly back as feedback with no delay at all. Depending on the feedback coefficient, that could cause instant instability.
The read-before-write order is not arbitrary — it ensures the delay is exactly D samples, not D-1.
```

## 43. Across Nodes 10, 11, and 12 — DelayLine, FeedbackDelay, and CombFilter — you have three classes where one wraps another. What is the design pattern being used, and what are its advantages over inheritance?
Composition

With composition, delay_line is private inside FeedbackDelay. The outside world cannot call delay_line.writeSample() directly. FeedbackDelay controls exactly what interface it exposes — processSample(), setDelay(), setFeedback(), setWetMix(). The internal DelayLine mechanics are hidden.


## 44. Your bandlimited square and saw wave generators normalize after the full buffer is generated. But your OnePoleLpf and BiquadFilter process sample by sample with no normalization. Why doesn't the filter need normalization but the oscillator does?
In One Pole LPF, the blend coefficient ensures that the sample stays in the correct range.. in the Biquad Filter, we already normalize using a0

## 45. Your ADSR uses a linear ramp for attack — currentValue += attackIncrement each sample. Professional synthesizers often use an exponential attack curve instead. Why might exponential feel more natural than linear for a volume envelope?
```
A linear attack ramp spends half its time crawling through the very quiet region (where every tiny change is very audible) and then rockets through the loud region (where large changes barely register). The result sounds like the attack accelerates — it seems slow at first then suddenly jumps to full volume.

An exponential curve does the opposite — it rises quickly through the quiet region and slows down as it approaches the peak. That matches how your ear weights loudness changes. The result sounds like a smooth, even rise in volume — which is what musicians mean when they say a sound has a "natural" attack.

This is why dB scale exists — it's just the logarithm of amplitude, calibrated to match human loudness perception.
```

## 46. Look at your pink_noise.cpp. The Voss-McCartney algorithm uses bit manipulation on a counter n to decide which rows to update:
```
uint32_t changed_bits = n ^ (n - 1);

What does n ^ (n-1) actually compute — which bits does it set — and why does that determine which rows to update?
```
Each bit represents a row.. the XOR operation tells us which bit has flipped and that lets us know which rows need to be updated
Each bit represents an octave

## 47. Across your entire codebase you have two architectural styles for classes: static methods (like WhiteNoise::generate()) vs instance methods with state (like OnePoleLpf). What determines which pattern is appropriate, and could you have implemented WhiteNoise as a stateful instance class instead?
White noise needs no memory between calls → static method
One-pole LPF needs memory between calls → stateful instance

Yes, white noise can be implemented as a stateful instance class as well

## 49. You've now built 12 nodes. Looking back at the full chain — white noise → oscillators → mixer → filters → ADSR → delay effects — describe in one paragraph how you could connect these existing components to build a simple synthesizer voice that plays a filtered, enveloped note. Which components do you use and in what order?
First generate some white noise and same with oscillators to generate different waves at different frequencies
Let's assume we generate a square wave. Then I would first apply a low pass filter to the white noise. This will add some more lower frequencies but also maintain some of the "hiss". Then mix this with the square wave.
Then use ADSR to manipulate the "volume" of the final buffer.

## 50. You've spent 12 nodes building audio DSP from scratch in C++. Node 13 is the Envelope Follower. Before I tell you anything about it — based on the name alone and everything you've learned so far, what do you think an envelope follower does, what problem does it solve, and which of your existing components does it most closely resemble architecturally?
An envelope follower tracks the amplitude of an incoming signal over time.

The one-pole LPF is the architectural ancestor of the envelope follower.