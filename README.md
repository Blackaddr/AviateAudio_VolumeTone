# MultiverseCommunity Volume Tone Example
This Effect Creator project demonstrates a very simple tone control. Guitar effects often include a simple 1st-order lowpass filter.

Low pass filters can be built many ways, but one convenient way that is computationally very lightweight is to use an allpass filter with feedback.

The following link describes how to create simple lowpass and high pass first order filters that provide recalculation of a single co-efficient as the user turns a knob.

[Allpass-Based Lowpass and Highpass Filters](http://https://thewolfsound.com/allpass-based-lowpass-and-highpass-filters/ "Allpass-Based Lowpass and Highpass Filters")

## License
Author: Steve Lascos
Public Domain

## Defining the Tone Control
In this example, we want a lowpass filter that has an adjustable cutoff frequency between 100 Hz and 2500 Hz.

## Mapping the Control
The Multiverse Framework (MVF) passes all user control values (i.e. knob value) as a normalized value between 0.0 and 1.0.

The MVF provides a convenience function to convert this normalized value back to the range you defined in the Effect Creator project. I.e. in this case, 100Hz to 2500 Hz.
```cpp
float cutoffHz = getUserParamValue(Tone_e, value);
```

Once we have target cutoff frequency in Hz, we can calculate the coefficient for our allpass filter. This is done whenever the user changes the tone control.
```cpp
float tmp = std::tan(M_PI * cutoffHz / AUDIO_SAMPLE_RATE_HZ);
float allpassCoeff = (tmp - 1.0f) / (tmp + 1.0f);
```

## Applying the Filter
Inside the loop of our update() function, we can calcualte the output of the allpass filter.
```cpp
float filterOutput = (m_tone * filterInput) + m_toneFeedback;
m_toneFeedback = sample - (m_tone * filterOutput);  // update the allpass feedback
```

For an allpass based LPF, we added the filter output back to the input and divide by 2.
```cpp
float outputSample = (filterInput + filterOutput)  0.5f;
```
