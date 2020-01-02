#include "base.h"
#include "bios.h"
#include "joystick.h"
#include "text.h"
#include "vectrexy.h"

void detail::AssertHandler(const char* cond, const char* file, size_t line) {
    vectrexy::printf("ASSERTION FAILED: %s [%s:%d]\n", cond, file, line);
    vectrexy::printf("Press all 4 buttons to continue\n", cond, file, line);

    // Display text on screen and wait for user to press continue input combo
    const Joystick& joystick1 = Joystick::Get(0);
    Text<8> assertText(-30, 0, "ASSERT!");
    while (true) {
        bios::WaitFrame();

        assertText.Draw();

        if (joystick1.IsDown(Button::B1) && joystick1.IsDown(Button::B2) &&
            joystick1.IsDown(Button::B3) && joystick1.IsDown(Button::B4))
            break;
    }
}
