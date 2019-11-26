#include "joystick.h"
#include "vectrex.h"

static bios::Dpad dpadtoBiosDpad[] = {
    bios::Dpad_Left,
    bios::Dpad_Right,
    bios::Dpad_Up,
    bios::Dpad_Down,
};

static bios::Button buttonToBiosButton[] = {
    bios::Button_1,
    bios::Button_2,
    bios::Button_3,
    bios::Button_4,
};

Joystick Joystick::joysticks[2] = {Joystick(0), Joystick(1)};

const Joystick& Joystick::Get(uint8_t playerIndex) {
    assert(playerIndex <= 1);
    return joysticks[playerIndex];
}

bool Joystick::IsDown(Dpad::Type dpad) const {
    return bios::IsDpadDown(m_playerIndex, dpadtoBiosDpad[dpad]);
}

bool Joystick::IsDown(Button::Type button) const {
    return bios::IsButtonDown(m_playerIndex, buttonToBiosButton[button]);
}

bool Joystick::IsPressed(Button::Type button) const {
    return bios::IsButtonPressed(m_playerIndex, buttonToBiosButton[button]);
}
