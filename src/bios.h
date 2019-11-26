#pragma once

#include "base.h"

// Wrapper around Vectrex bios routines
namespace bios {
    const int8_t DefaultIntensity = 127; // [0-127], default: max intensity
    const uint8_t DefaultScale = 127;    // [0-255], default: Init_VIA resets to this value

    // Call once at startup
    void Init();

    // Call once per frame, beam will be reset to 0,0
    void WaitFrame();

    // Resets beam to 0,0
    void ZeroBeam();

    // Set intensity from 0 to 127 (brightest)
    void SetBeamIntensity(int8_t intensity);

    // Set scale from 0 to 127
    void SetScale(uint8_t scale);

    // Draw vector list
    void Draw(const int8_t* const& vectorList, int8_t relx = 0, int8_t rely = 0,
              uint8_t scale = DefaultScale);
} // namespace bios
