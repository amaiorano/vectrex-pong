#pragma once

#include "base.h"
#include "simple_allocator.h"
#include "bios.h"

// Structure of vector list data
struct VectorListData {
    static SimpleAllocator<60> s_simpleAllocator;

    VectorListData() {}

    static VectorListData& FromMemory(int8_t* volatile memory) {
        return *reinterpret_cast<VectorListData*>(memory);
    }

    // Create new VectorListData from array of (y,x) offsets
    static VectorListData* Create(int8_t* vertices, int8_t size) {
        VectorListData* data = (VectorListData*)s_simpleAllocator.Allocate(size + 1);
        data->count = size / 2 - 1;
        memcpy(data->vertices, vertices, size);
        return data;
    }

    const int8_t* Data() const { return reinterpret_cast<const int8_t*>(this); }

private:
    // Number of vertices - 1
    int8_t count;
    // Array of (y,x) offsets to move beam
    int8_t vertices[];
};

// template <size_t val>
// struct PrintVal;
// PrintVal<sizeof(VectorListData)> a;

class VectorList {
public:
    VectorList(VectorListData& data)
        : m_data(data) {}

    void Draw() { bios::Draw(m_data.Data()); }

private:
    VectorListData& m_data;
};