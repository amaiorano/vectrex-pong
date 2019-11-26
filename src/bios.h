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

    enum Dpad { Dpad_Left, Dpad_Right, Dpad_Up, Dpad_Down };
    bool IsDpadDown(uint8_t playerIndex, Dpad dpad);

    enum Button { Button_1 = 0b0001, Button_2 = 0b0010, Button_3 = 0b0100, Button_4 = 0b1000 };
    bool IsButtonDown(uint8_t playerIndex, Button button);
    bool IsButtonPressed(uint8_t playerIndex, Button button);

} // namespace bios
