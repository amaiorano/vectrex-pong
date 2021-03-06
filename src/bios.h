#pragma once

#include "base.h"

// Wrapper around Vectrex bios routines
namespace bios {
    const int8_t DefaultIntensity = 127; // [0-127], default: max intensity
    const uint8_t DefaultScale = 127;    // [0-255], default: Init_VIA resets to this value

    // Call once at startup
    void Init();

    // Show the screen where player can select number of players and the game version.
    // This is a blocking call, and returns once the user pressed button 4, or enough time elapses.
    void ShowSelectGameScreen(uint8_t maxNumPlayers, uint8_t numGameModes,
                              uint8_t& selectedNumPlayers, uint8_t& selectedGameMode);

    // Call once per frame, beam will be reset to 0,0
    void WaitFrame();

    // Resets beam to 0,0
    void ZeroBeam();

    // Set intensity from 0 to 127 (brightest)
    void SetBeamIntensity(int8_t intensity);

    // Set scale from 0 to 127
    void SetScale(uint8_t scale);

    // Draw vector list
    void Draw(const void* vectorList, int8_t relx = 0, int8_t rely = 0,
              uint8_t scale = DefaultScale);

    void PrintString(const void* block);

    enum Dpad { Dpad_Left, Dpad_Right, Dpad_Up, Dpad_Down };
    bool IsDpadDown(uint8_t playerIndex, Dpad dpad);

    enum Button { Button_1 = 0b0001, Button_2 = 0b0010, Button_3 = 0b0100, Button_4 = 0b1000 };
    bool IsButtonDown(uint8_t playerIndex, Button button);
    bool IsButtonPressed(uint8_t playerIndex, Button button);

    // Play one of the 13 music entries in the bios. Will stop sound, if any.
    void PlayMusic(uint8_t musicIndex);
    void StopMusic();
    bool IsPlayingMusic();

    // Play sound defined by block. Will stop music, if any.
    // For now, sounds played on channel A and B will have their tone increase quickly until a
    // threshold is hit, and then the channel is muted.
    void PlaySound(const void* soundBlock);
    void StopSound();

} // namespace bios
