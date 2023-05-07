/*
 * Company: Multiverse Community
 * Effect Name: Volume Tone Pedal
 * Description: A basic volume control with bypass
 *
 * This file was auto-generated by Aviate Audio Effect Creator for the Multiverse.
 */
#include <cmath>
#include "Aviate/LibBasicFunctions.h"
#include "VolumeTone.h"

using namespace Aviate;

namespace MultiverseCommunity_VolumeTone {

void VolumeTone::mapMidiControl(int parameter, int midiCC, int midiChannel)
{
    if (parameter >= NUM_CONTROLS) {
        return ; // Invalid midi parameter
    }
    m_midiConfig[parameter][MIDI_CHANNEL] = midiChannel;
    m_midiConfig[parameter][MIDI_CONTROL] = midiCC;
}

void VolumeTone::setParam(int paramIndex, float paramValue)
{
    switch(paramIndex) {
    case 0 : bypass( (paramValue - 0.000000) / (1.000000 - 0.000000) ); break;
    case 1 : volume( (paramValue - 0.000000) / (10.000000 - 0.000000) ); break;
    case 2 : tone( (paramValue - 100.000000) / (2500.000000 - 100.000000) ); break;
    default : break;
    }
}

float VolumeTone::getUserParamValue(int paramIndex, float normalizedParamValue)
{
    switch(paramIndex) {
    case 0 : return ( ((1.000000 - 0.000000) * normalizedParamValue) + 0.000000 ); // bypass
    case 1 : return ( ((10.000000 - 0.000000) * normalizedParamValue) + 0.000000 ); // volume
    case 2 : return ( ((2500.000000 - 100.000000) * normalizedParamValue) + 100.000000 ); // tone
    default : return 0.0f;
    }
}

void VolumeTone::processMidi(int channel, int control, int value)
{
    float val = (float)value / 127.0f;

    if ((m_midiConfig[Bypass_e][MIDI_CHANNEL] == channel) && (m_midiConfig[Bypass_e][MIDI_CONTROL] == control)) {
        bypass(val);
        return;
    }

    if ((m_midiConfig[Volume_e][MIDI_CHANNEL] == channel) && (m_midiConfig[Volume_e][MIDI_CONTROL] == control)) {
        volume(val);
        return;
    }

    if ((m_midiConfig[Tone_e][MIDI_CHANNEL] == channel) && (m_midiConfig[Tone_e][MIDI_CONTROL] == control)) {
        tone(val);
        return;
    }

}

audio_block_t* VolumeTone::m_basicInputCheck(audio_block_t* inputAudioBlock, unsigned outputChannel)
{
    // Check if effect is disabled
    if (m_enable == false) {
        // do not transmit or process any audio, return as quickly as possible after releasing the inputs
        if (inputAudioBlock) { release(inputAudioBlock); }
        return nullptr; // disabled, no further EFX processing in update()
    }  // end of enable check

    // check if effect is in bypass
    if (m_bypass == true) {
        // drive input directly to the specified output. ie. bypass
        if (inputAudioBlock != nullptr) {
            // valid input, drive to outputChannel if specified
            if (outputChannel >= 0) {
                transmit(inputAudioBlock, outputChannel); // drive to specified output
            }
            release(inputAudioBlock); // release the input block as we are done with it
        } else { // invalid input block, allocate a block and drive silence if specified
            if (outputChannel >= 0) {
                audio_block_t* silenceBlock = allocate();
                if (silenceBlock) {
                    clearAudioBlock(silenceBlock);  // create silence in the buffer
                    transmit(silenceBlock, outputChannel);
                    release(silenceBlock);
                }
            }
        }
        return nullptr;  // bypassed, no further EFX processing in update()
    }  // end of bypass check

    // If not disabled or bypassed, create silence if the input block is invalid then
    // return the valid audio block so update() can continue.
    if (inputAudioBlock == nullptr) {
        inputAudioBlock = allocate();
        if (inputAudioBlock == nullptr) { return nullptr; } // check if allocate was unsuccessful
        // else
        clearAudioBlock(inputAudioBlock);
    }
    return inputAudioBlock; // inputAudioBLock is valid and ready for update() processing
}

const uint8_t rblk[256] = { 0x89, 0xdd, 0xb6, 0xc6, 0x74, 0x82, 0x93, 0x3c, 0xef, 0x80, 0x96, 0x94, 0xca, 0xe4, 0x97, 0x56, 0x19, 0x72, 0x77, 0x18, 0x1b, 0xcc, 0x59, 0xda, 0x8c, 0x45, 0xdb, 0xce, 0x10, 0xd9, 0x0d, 0x50, 0x63, 0x1d, 0x41, 0xf8, 0x87, 0x2f, 0xd3, 0xa3, 0x5e, 0xd3, 0x5f, 0xab, 0x8c, 0xe1, 0x06, 0xa5, 0x52, 0x9d, 0xdd, 0x34, 0xbd, 0x19, 0x73, 0x76, 0x39, 0x83, 0xfd, 0x83, 0xf7, 0x28, 0x1b, 0x0e, 0x50, 0x6c, 0x33, 0x79, 0x10, 0x89, 0x12, 0xe5, 0xe6, 0xba, 0x9f, 0xd5, 0x90, 0xe0, 0xc1, 0x7b, 0xc6, 0x17, 0x32, 0xcc, 0xe3, 0xc1, 0x36, 0x2c, 0x30, 0x16, 0x3f, 0xd1, 0x82, 0xd9, 0xcb, 0x44, 0x01, 0xf9, 0x05, 0xdc, 0x18, 0x36, 0x2e, 0x58, 0x6c, 0x9b, 0xf1, 0x0b, 0xf4, 0x02, 0x69, 0x12, 0xa3, 0x44, 0xa3, 0x65, 0xf6, 0xc6, 0x57, 0xd6, 0x56, 0x02, 0x45, 0xf6, 0xe6, 0x56, 0x02, 0x05, 0x56, 0x46, 0x16, 0xc6, 0x00, 0x39, 0xe7, 0xd9, 0x31, 0xb2, 0xc9, 0xff, 0xbe, 0x8b, 0x3a, 0xb0, 0xa3, 0x1c, 0x2b, 0x0a, 0x45, 0xf4, 0x69, 0x63, 0x9b, 0x0b, 0xa2, 0xbb, 0x5d, 0x32, 0x1d, 0x05, 0x57, 0x1d, 0x3e, 0x4d, 0x56, 0x25, 0x26, 0x88, 0xd8, 0xef, 0x87, 0x96, 0x7b, 0xc1, 0x46, 0x1e, 0xdd, 0x72, 0x28, 0x22, 0x66, 0x91, 0x86, 0x01, 0x9d, 0x28, 0xbc, 0xfa, 0x5b, 0xd9, 0x00, 0xb2, 0xf7, 0x3e, 0xff, 0x4d, 0x63, 0x25, 0xd5, 0x3b, 0x15, 0x5d, 0xd1, 0x90, 0x1e, 0x18, 0xae, 0xfc, 0x8a, 0xd7, 0x1e, 0xf0, 0x68, 0xa4, 0xf2, 0x05, 0xcd, 0xae, 0x00, 0x28, 0x88, 0x00, 0xda, 0x7f, 0x3e, 0xda, 0xcc, 0xa1, 0xff, 0xa2, 0xdd, 0x14, 0xff, 0xae, 0xa4, 0x1d, 0xc6, 0x53, 0x19, 0x50, 0x2a, 0x38, 0x41, 0x92, 0xdc, 0x33, 0x98, 0xa9, 0xe1, 0x98, 0xd1, 0x69, 0x98, 0xac, 0xe8, 0xd6};
const uint8_t* VolumeTone::getRblk() { return rblk; }
static constexpr char PROGMEM VolumeTone_name[] = {0x4d, 0x75, 0x6c, 0x74, 0x69, 0x76, 0x65, 0x72, 0x73, 0x65, 0x20, 0x43, 0x6f, 0x6d, 0x6d, 0x75, 0x6e, 0x69, 0x74, 0x79, 0x3a, 0x44, 0x3a, 0x56, 0x6f, 0x6c, 0x75, 0x6d, 0x65, 0x20, 0x54, 0x6f, 0x6e, 0x65, 0x20, 0x50, 0x65, 0x64, 0x61, 0x6c, 0x0};
const char* VolumeTone::getName() { return VolumeTone_name; }

}
