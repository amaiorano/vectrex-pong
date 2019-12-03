#include "bios.h"
#include "vectrex.h"

namespace bios {

    void Init() {
        SetBeamIntensity(DefaultIntensity);
        SetScale(DefaultScale);

        // Enable reading analog/digital values of both joysticks
        // TODO: configurable to save some cycles if not all are needed.
        Vec_Joy_Mux_1_X = 1;
        Vec_Joy_Mux_1_Y = 3;
        Vec_Joy_Mux_2_X = 5;
        Vec_Joy_Mux_2_Y = 7;
    }

    void ShowSelectGameScreen(uint8_t maxNumPlayers, uint8_t numGameModes,
                              uint8_t& selectedNumPlayers, uint8_t& selectedGameMode) {
        Select_Game(maxNumPlayers, numGameModes);
        selectedNumPlayers = Vec_Num_Players;
        selectedGameMode = Vec_Num_Game;
    }

    void WaitFrame() {
        Wait_Recal();

        // Read joysticks
        Read_Btns();
        Joy_Digital();

        // Need to set beam intensity after Wait_Recal.
        // In fact, Joy_Digital also seems to reset intensity to 0.
        // We may need to call this each time we draw.
        SetBeamIntensity(DefaultIntensity);
    }

    void ZeroBeam() { Reset0Ref(); }

    void SetBeamIntensity(int8_t intensity) { Intensity_a(intensity); }

    void SetScale(uint8_t scale) { VIA_t1_cnt_lo = scale; }

    void Draw(const void* vectorList, int8_t relx, int8_t rely, uint8_t scale) {
        SetScale(scale);
        Moveto_d(relx, rely);
        Draw_VLc(vectorList);
    }

    void PrintString(const void* block) { Print_Str_hwyx(block); }

    bool IsDpadDown(uint8_t playerIndex, Dpad dpad) {
        static const uint8_t mask[4] = {
            0x80, // Left
            0x00, // Right
            0x00, // Up
            0x80, // Down
        };

        // Compute the offset to the register, and check the sign bit
        volatile uint8_t* state = &Vec_Joy_1_X + (dpad / 2) + (playerIndex * 2);
        return (*state != 0) && ((*state) & 0x80) == mask[dpad];
    }

    bool IsButtonDown(uint8_t playerIndex, Button button) {
        uint8_t mask = (uint8_t)button;
        if (playerIndex) {
            mask <<= 4;
        }
        return (Vec_Btn_State & mask) != 0;
    }

    bool IsButtonPressed(uint8_t playerIndex, Button button) {
        uint8_t mask = (uint8_t)button;
        if (playerIndex) {
            mask <<= 4;
        }
        return (Vec_Buttons & mask) != 0;
    }
} // namespace bios
