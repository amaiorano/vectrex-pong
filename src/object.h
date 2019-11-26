#pragma once

#include "base.h"
#include "vector_list.h"

class Object {
public:
    Object(const VectorList& vectorList)
        : m_vectorList(&vectorList)
        , m_x(0)
        , m_y(0)
        , m_scale(bios::DefaultScale) {
        m_vectorList->computeExtents(m_extents.x1, m_extents.x2, m_extents.y1, m_extents.y2);
    }

    void Draw() {
        bios::ZeroBeam(); // @TODO: optimize chaining moves between objects
        bios::Draw(m_vectorList->Data(), (int8_t)m_x, (int8_t)m_y, m_scale);
    }

    void SetPos(int8_t x, int8_t y) {
        m_x = x;
        m_y = y;
    }

    void Move(int8_t relx, int8_t rely) {
        m_x += relx;
        m_y += rely;
    }

    // void SetScale(uint8_t scale) { m_scale = scale; }

    int8_t X() const { return m_x; }
    int8_t Y() const { return m_y; }

    int8_t Left() const { return m_x + m_extents.x1; }
    int8_t Right() const { return m_y + m_extents.x2; }
    int8_t Top() const { return m_y + m_extents.y1; }
    int8_t Bottom() const { return m_y + m_extents.y2; }

private:
    const VectorList* const m_vectorList;
    struct Extents {
        int8_t x1, y1; // Top-left
        int8_t x2, y2; // Bottom-right
    } m_extents;

    int8_t m_x, m_y;
    uint8_t m_scale;
};
