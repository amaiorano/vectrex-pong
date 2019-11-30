#pragma once

#include "base.h"

namespace bios {
    void ZeroBeam();
    void PrintString(const void* block);
} // namespace bios

template <uint8_t MaxTextLength>
class Text {
public:
    static const uint8_t DefaultCharWidth = 50;
    static const uint8_t DefaultCharHeight = 5;

    Text(uint8_t x, uint8_t y, const char* string = "", uint8_t charWidth = DefaultCharWidth,
         uint8_t charHeight = DefaultCharHeight);

    void SetString(const char* string);
    void SetSize(uint8_t charWidth = DefaultCharWidth, uint8_t charHeight = DefaultCharHeight);
    size_t Length() const;
    void Draw() const;

private:
    static const char Terminator = 0x80;

    // Text data laid out as required by Print_List_*
    struct Data {
        uint8_t height;
        uint8_t width;
        uint8_t rely;
        uint8_t relx;
        char string[MaxTextLength + 1]; // Terminated by 0x80
    } m_data;
};

// Inline implementation
template <uint8_t MaxTextLength>
Text<MaxTextLength>::Text(uint8_t x, uint8_t y, const char* string, uint8_t charWidth,
                          uint8_t charHeight) {
    m_data.height = charHeight * -1;
    m_data.width = charWidth;
    m_data.rely = y;
    m_data.relx = x;
    SetString(string);
}

template <uint8_t MaxTextLength>
void Text<MaxTextLength>::SetString(const char* string) {
    size_t len = strlen(string);
    memcpy(m_data.string, string, len);
    assert(len <= MaxTextLength);
    m_data.string[len] = Terminator;

    // Workaround a bug in the Vectrex's text drawing BIOS routine that fails to draw a single
    // character correctly. If we add a space after it, it renders properly.
    if (len == 1) {
        assert(MaxTextLength >= 2);
        m_data.string[1] = ' ';
        m_data.string[2] = Terminator;
    }
}

template <uint8_t MaxTextLength>
void Text<MaxTextLength>::SetSize(uint8_t charWidth, uint8_t charHeight) {
    m_data.height = charHeight * -1;
    m_data.width = charWidth;
}

template <uint8_t MaxTextLength>
size_t Text<MaxTextLength>::Length() const {
    const char* s = m_data.string;
    while (*s != Terminator) {
        ++s;
    }
    return s - m_data.string;
}

template <uint8_t MaxTextLength>
void Text<MaxTextLength>::Draw() const {
    bios::ZeroBeam(); // @TODO: optimize chaining moves between objects
    bios::PrintString(reinterpret_cast<const uint8_t*>(&m_data));
}
