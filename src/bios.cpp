#include "bios.h"
#include "vectrex.h"

namespace PsgReg {
    enum Type {
        ToneALow = 0,
        ToneAHigh = 1,
        ToneBLow = 2,
        ToneBHigh = 3,
        ToneCLow = 4,
        ToneCHigh = 5,
        Noise = 6,
        MixerControl = 7,
        AmplitudeA = 8,
        AmplitudeB = 9,
        AmplitudeC = 10,
        EnvelopePeriodLow = 11,
        EnvelopePeriodHigh = 12,
        EnvelopeShape = 13,
        IOPortADataStore = 14,
        IOPortBDataStore = 15
    };
}

namespace {
    void* g_currMusic = nullptr;
    uint8_t g_last_Vec_Music_Flag = Vec_Music_Flag;

    static void UpdateAudio() {
        // Must call Init_Music_chk every frame to have it advance the current music to the PSG
        // NOTE: might have to do this just before Wait_Recal
        if (Vec_Music_Flag != 0) {
            DP_to_C8();
            Init_Music_chk(g_currMusic);
            DP_to_D0();
        }

        // When music stops playing, we need to clear the audio chip registers so that we don't
        // hear the last state
        if (Vec_Music_Flag == 0 && g_last_Vec_Music_Flag != 0) {
            g_currMusic = nullptr;
            Clear_Sound();
        }
        g_last_Vec_Music_Flag = Vec_Music_Flag;

        if (Vec_Music_Flag != 0) {
            // Update PSG with latest music
            Do_Sound();
        } else {
            // Update sound
            // This is adapted from Mine Storm, which "slides" Tone A and B channel frequencies
            // up, and mute the channel after hitting a threshold. We may want to do something
            // more general/programmable.
            {
                volatile uint8_t* sndShadow = &Vec_Snd_Shadow;

                uint16_t toneA = sndShadow[PsgReg::ToneALow];
                toneA += 16;
                if (toneA >= 160) {
                    Sound_Byte(PsgReg::AmplitudeA, 0);
                } else {
                    Sound_Byte(0, (uint8_t)toneA);
                }

                uint16_t toneB = sndShadow[PsgReg::ToneBLow];
                toneB += 32;
                if (toneB >= 240) {
                    Sound_Byte(PsgReg::AmplitudeB, 0);
                } else {
                    Sound_Byte(2, (uint8_t)toneB);
                }
            }
        }
    }
} // namespace

namespace bios {
    void Init() {
        SetBeamIntensity(DefaultIntensity);
        SetScale(DefaultScale);

        StopMusic();
        StopSound();

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
        DP_to_D0();
        selectedNumPlayers = Vec_Num_Players;
        selectedGameMode = Vec_Num_Game;
    }

    void WaitFrame() {
        Wait_Recal();

        UpdateAudio();

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

    void PlayMusic(uint8_t musicIndex) {
        static uint16_t musics[] = {
            music1, music2, music3, music4, music5, music6, music7,
            music8, music9, musica, musicb, musicc, musicd,
        };
        assert(musicIndex < std::size(musics));

        // Note that we don't bother to StopSound() here because music fills up the entire PSG
        // register set.
        Vec_Music_Flag = 1;
        g_currMusic = (void*)musics[musicIndex];
    }

    void StopMusic() {
        if (Vec_Music_Flag != 0) {
            Vec_Music_Flag = 0;
            g_last_Vec_Music_Flag = 0;
            g_currMusic = nullptr;
            Clear_Sound();
        }
    }

    bool IsPlayingMusic() { return Vec_Music_Flag != 0; }

    void PlaySound(const void* soundBlock) {
        StopMusic();
        Sound_Bytes(soundBlock);
    }

    void StopSound() { Clear_Sound(); }
} // namespace bios
