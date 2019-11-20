#pragma once

#include "base.h"
#include "vectrex.h" // TODO: move to cpp, along with impls of bios functions below

// Wrapper around Vectrex bios routines
namespace bios {
    const uint8_t DefaultIntensity = 127;
    const uint8_t DefaultScale = 200;

    // Call once at startup
    void Init();

    // Call once per frame, beam will be reset to 0,0
    void WaitFrame();

    // Resets beam to 0,0
    void ZeroBeam();

    // Set intensity from 0 to 127 (brightest)
    void SetBeamIntensity(uint8_t intensity);

    // Set scale from 0 to 127
    void SetScale(uint8_t scale);

    // Draw vector list
    void Draw(const int8_t* const& vectorList, uint8_t relx = 0, uint8_t rely = 0,
              uint8_t scale = DefaultScale);
} // namespace bios
