#pragma once
#include "base.h"

// This file contains macros that can be used to help define a sound block. Sound blocks should be
// declared as a static const array of uint8_t, to make sure the block is in rom, and not in ram.
//
// Example usage:
//
// This defines a sound block that enables a single tone on channel B, with a volume envelope that
// drops down quickly.
//
// static const uint8_t paddleHitSound[] = {PSG_TONE(0, 192, 0),
//                                          PSG_NOISE(0),
//                                          PSG_MIXER(PsgMixer::ToneB),
//                                          PSG_AMPLITUDE(0, 31, 0),
//                                          PSG_ENVELOPE(3327, PsgEnvelopeShape::Down),
//                                          PSG_TERMINATOR};

namespace PsgMixer {
    // Flags
    const uint8_t ToneA = 1 << 0;
    const uint8_t ToneB = 1 << 1;
    const uint8_t ToneC = 1 << 2;
    const uint8_t NoiseA = 1 << 3;
    const uint8_t NoiseB = 1 << 4;
    const uint8_t NoiseC = 1 << 5;
} // namespace PsgMixer

namespace PsgEnvelopeShape {
    const uint8_t Down = 0;
    const uint8_t Up = 4;
    const uint8_t DownUp = 8;
    const uint8_t UpDown = 12;
}; // namespace PsgEnvelopeShape

#define _PSG_LOW(value16) ((uint16_t)value16 & 0x00ff)
#define _PSG_HIGH(value16) (((uint16_t)value16 >> 8) & 0x00ff)

#define PSG_TONE_A(value16) 0x00, _PSG_LOW(value16), 0x01, _PSG_HIGH(value16)
#define PSG_TONE_B(value16) 0x02, _PSG_LOW(value16), 0x03, _PSG_HIGH(value16)
#define PSG_TONE_C(value16) 0x04, _PSG_LOW(value16), 0x05, _PSG_HIGH(value16)
#define PSG_NOISE(value8) 0x06, value8
#define PSG_MIXER(psgMixerMask) 0x07, (~(psgMixerMask)&0b00111111)
#define PSG_AMPLITUDE_A(value8) 0x08, value8
#define PSG_AMPLITUDE_B(value8) 0x09, value8
#define PSG_AMPLITUDE_C(value8) 0x0A, value8
#define PSG_ENVELOPE_PERIOD(value16) 0x0B, _PSG_LOW(value16), 0x0C, _PSG_HIGH(value16)
#define PSG_ENVELOPE_SHAPE(psgEnvelopeShape) 0x0D, static_cast<uint8_t>(psgEnvelopeShape)
#define PSG_TERMINATOR 0xFF

// Convenience macros for setting multiple values with one macro
#define PSG_TONE(a16, b16, c16) PSG_TONE_A(a16), PSG_TONE_B(b16), PSG_TONE_C(c16)
#define PSG_AMPLITUDE(a8, b8, c8) PSG_AMPLITUDE_A(a8), PSG_AMPLITUDE_B(b8), PSG_AMPLITUDE_C(c8)
#define PSG_ENVELOPE(period16, shape) PSG_ENVELOPE_PERIOD(period16), PSG_ENVELOPE_SHAPE(shape)
