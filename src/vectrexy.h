#pragma once

#include "base.h"

// This header contains development utilities that can be used with the Vectrexy emulator.

namespace vectrexy {
    // printf is similar to the <cstdio> version, and is very efficient as all the work is offloaded
    // to the emulator. With inlining, the generated asm consists of simply assigning the format
    // string and args to custom memory-mapped registers.
    //
    // It comes with the following restrictions :
    // * Format specifiers: %d, %u, %c, %s work fine, along with width modifiers, etc. %f does
    // not work because floats are not supported by gcc6809.
    // * Args can be of any integral type that is 8-bit or 16-bit wide, or pointer type
    // (16-bit).
    template <typename A1>
    void printf(const char* format, A1 a1);
    template <typename A1, typename A2>
    void printf(const char* format, A1 a1, A2 a2);
    template <typename A1, typename A2, typename A3>
    void printf(const char* format, A1 a1, A2 a2, A3 a3);
    template <typename A1, typename A2, typename A3, typename A4>
    void printf(const char* format, A1 a1, A2 a2, A3 a3, A4 a4);
    template <typename A1, typename A2, typename A3, typename A4, typename A5>
    void printf(const char* format, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5);

} // namespace vectrexy

// Memory-mapped development registers, known specifically by Vectrexy. These are not supported by
// the actual Vectrex, nor by any other emulator (yet?). These fall in the unmapped 2K memory area
// of [0xC000, 0xC7FF].
#define DEV_PRINTF_PUSH_ARG8 *((volatile int8_t*)0xC100)
#define DEV_PRINTF_PUSH_ARG16 *((volatile int16_t*)0xC101)
#define DEV_PRINTF_PUSH_CSTR *((volatile uint8_t * volatile*)0xC103)
#define DEV_PRINTF_FORMAT *((volatile const char* volatile*)0xC105)

// Inline implementation
namespace vectrexy {
    namespace detail {
        template <typename T>
        struct PushArg;

        template <>
        struct PushArg<uint8_t> {
            static void Exec(uint8_t v) { DEV_PRINTF_PUSH_ARG8 = v; }
        };

        template <>
        struct PushArg<int8_t> {
            static void Exec(int8_t v) { DEV_PRINTF_PUSH_ARG8 = v; }
        };

        template <>
        struct PushArg<char> {
            static void Exec(char v) { DEV_PRINTF_PUSH_ARG8 = v; }
        };

        template <>
        struct PushArg<uint16_t> {
            static void Exec(uint16_t v) { DEV_PRINTF_PUSH_ARG16 = v; }
        };

        template <>
        struct PushArg<int16_t> {
            static void Exec(int16_t v) { DEV_PRINTF_PUSH_ARG16 = v; }
        };

        template <>
        struct PushArg<const char*> {
            static void Exec(const char* p) { DEV_PRINTF_PUSH_CSTR = (uint8_t*)p; }
        };
    } // namespace detail

    template <typename A1>
    void printf(const char* format, A1 a1) {
        detail::PushArg<A1>::Exec(a1);
        DEV_PRINTF_FORMAT = format;
    }

    template <typename A1, typename A2>
    void printf(const char* format, A1 a1, A2 a2) {
        detail::PushArg<A1>::Exec(a1);
        detail::PushArg<A2>::Exec(a2);
        DEV_PRINTF_FORMAT = format;
    }

    template <typename A1, typename A2, typename A3>
    void printf(const char* format, A1 a1, A2 a2, A3 a3) {
        detail::PushArg<A1>::Exec(a1);
        detail::PushArg<A2>::Exec(a2);
        detail::PushArg<A3>::Exec(a3);
        DEV_PRINTF_FORMAT = format;
    }

    template <typename A1, typename A2, typename A3, typename A4>
    void printf(const char* format, A1 a1, A2 a2, A3 a3, A4 a4) {
        detail::PushArg<A1>::Exec(a1);
        detail::PushArg<A2>::Exec(a2);
        detail::PushArg<A3>::Exec(a3);
        detail::PushArg<A4>::Exec(a4);
        DEV_PRINTF_FORMAT = format;
    }

    template <typename A1, typename A2, typename A3, typename A4, typename A5>
    void printf(const char* format, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
        detail::PushArg<A1>::Exec(a1);
        detail::PushArg<A2>::Exec(a2);
        detail::PushArg<A3>::Exec(a3);
        detail::PushArg<A4>::Exec(a4);
        detail::PushArg<A5>::Exec(a5);
        DEV_PRINTF_FORMAT = format;
    }
} // namespace vectrexy
