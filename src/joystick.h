#pragma once

#include "base.h"
#include "bios.h"

namespace Dpad {
    enum Type { Left, Right, Up, Down };
}

namespace Button {
    enum Type { B1, B2, B3, B4 };
}

class Joystick {
public:
    static const Joystick& Get(uint8_t playerIndex);

    bool IsDown(Dpad::Type dpad) const;

    bool IsDown(Button::Type button) const;
    bool IsPressed(Button::Type button) const;

private:
    Joystick(uint8_t playerIndex)
        : m_playerIndex(playerIndex) {}

    static Joystick joysticks[2];
    uint8_t m_playerIndex;
};
