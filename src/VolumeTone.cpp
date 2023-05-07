/*
 * Company: Multiverse Community
 * Effect Name: Volume Tone Pedal
 * Description: A basic volume and tone control with bypass
 *
 * This effect uses the allpass feedback circuit to create a compute
 * efficient 1st-order low-pass filter.
 *
 * Reference: https://thewolfsound.com/allpass-based-lowpass-and-highpass-filters/
 */
 #include <cmath> // for std::tan() and M_PI
#include "Aviate/EfxPrint.h"
#include "VolumeTone.h"

using namespace Aviate;

namespace MultiverseCommunity_VolumeTone {

VolumeTone::VolumeTone()
: AudioStream(NUM_INPUTS, m_inputQueueArray)
{
    // perform any necessary class initialization here
}

VolumeTone::~VolumeTone()
{
    // perform any necessary clean up here, though destructors are not
    // called on the hardware, only in the simulator.
}

void VolumeTone::update(void)
{
    audio_block_t *inputAudioBlock = receiveWritable(); // get the next block of input samples
    inputAudioBlock = m_basicInputCheck(inputAudioBlock, 0); // check for disable mode, bypass, or invalid inputs. Transmit to channel 0 in bypass
    if (!inputAudioBlock) { return; } // no further processing for this update() call

    // You must call m_updateInputPeak() before processing the audio
    m_updateInputPeak(inputAudioBlock);

    // DO AUDIO EFFECT PROCESSING
    for (auto idx=0; idx<AUDIO_BLOCK_SAMPLES; idx++) {

        float sample = (float)inputAudioBlock->data[idx];
        
        // Calculate the allpass output
        float filterOutput = (m_tone * sample) + m_toneFeedback;
        m_toneFeedback = sample - (m_tone * filterOutput);  // update the allpass feedback

        // Sum the input and the allpass to get the low pass filter,
        // then apply volume and gain correction (divided by two due to signal summation)
        float outputSample = (sample + filterOutput) * m_volume * 0.5f;
        inputAudioBlock->data[idx] = (int16_t)outputSample; // we re-use the input audio block
    }

    m_updateOutputPeak(inputAudioBlock); // you must call m_upateOutputPeak() at the end of update() before transmit
    transmit(inputAudioBlock);
    release(inputAudioBlock);
}

// input value for volume() is normalized from 0 to 1.0. Convert to a dB value between
// -40 dB and +10 dB
void VolumeTone::volume(float value)
{
    float volDbValue = -40.0f + (value * 50.0f);  // remap the normalized value to represent -40dB to +10dB
    volumeDb(volDbValue);  // AudioEffectWrapper has built-in volume function in dB

    // Demonstrate the Serial.printf() function to print over the USB serial port
    // EFX_PRINT must be used to ensure prints only appear in DEBUG builds.
    EFX_PRINT(Serial.printf("volume: normalized value: %f  dB value: %f\n\r", value, volDbValue));
}

// input value for tone() is normalized from 0 to 1.0. Convert to a cutoff frequency in Hz,
// then calculate the allpass coefficient.
void VolumeTone::tone(float value)
{
    // Convert normalized value back to Hz unit
    float cutoffHz = getUserParamValue(Tone_e, value);
    
    // Calculate the Allpass coefficient
    float tmp = std::tan(M_PI * cutoffHz / AUDIO_SAMPLE_RATE_HZ);
    float allpassCoeff = (tmp - 1.0f) / (tmp + 1.0f);
    m_tone = allpassCoeff;
    EFX_PRINT(Serial.printf("tone: normalized value: %f  tone: %f\n\r", value, m_tone));
}

}
