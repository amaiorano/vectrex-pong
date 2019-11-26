#pragma once

#include "base.h"
#include "bios.h"
#include "simple_allocator.h"

class VectorList {
public:
    typedef SimpleAllocator<20> AllocatorType;
    static AllocatorType s_simpleAllocator;

    VectorList() {}

    static VectorList& FromMemory(int8_t* volatile memory) {
        return *reinterpret_cast<VectorList*>(memory);
    }

    // Create new VectorList from array of (y,x) offsets
    static VectorList* Create(int8_t* vertices, int8_t size) {
        VectorList* data = (VectorList*)s_simpleAllocator.Allocate(size + 1);
        data->count = size / 2 - 1;
        memcpy(data->vertices, vertices, size);
        return data;
    }

    const int8_t* Data() const { return reinterpret_cast<const int8_t*>(this); }

    // (x1,y1) are set to min extents (bottom-left), and (x2,y2) are set to max extents (top-right)
    void computeExtents(int8_t& x1, int8_t& x2, int8_t& y1, int8_t& y2) const {
        x1 = y2 = 127;
        x2 = y1 = -128;
        int8_t x = 0, y = 0;
        int8_t size = (count + 1) * 2;
        for (int8_t i = 0; i < size; i += 2) {
            y += vertices[i]; // Move to next y position
            y1 = max(y1, y);
            y2 = min(y2, y);
            x += vertices[i + 1]; // Move to next x position
            x1 = min(x1, x);
            x2 = max(x2, x);
        }
    }

private:
    // Number of (y,x) vertices - 1
    int8_t count;
    // Array of (y,x) offsets to move beam
    int8_t vertices[];
};
