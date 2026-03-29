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
