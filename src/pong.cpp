// Pong

#include "vectrex.h"

#define assert(cond)

// Use CONCAT to concatenate names with macros like __COUNTER__
#define CONCAT_(x, y) x##y
#define CONCAT(x, y) CONCAT_(x, y)

// static_assert produces a compile-time error if cond is false, otherwise it creates an instance of
// a 0-byte sized type
template <bool cond>
struct static_assert_impl {
    char static_assert_failed[cond ? 0 : -1];
};

#define static_assert(cond) static_assert_impl < cond > CONCAT(static_assert_, __COUNTER__)

typedef unsigned long int uint16_t;
typedef signed long int int16_t;
typedef uint16_t size_t;

static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(int16_t) == 2);

// Default placement new. Simply returns second argument.
void* operator new(size_t, void* mem) {
    return mem;
}

void* memcpy(void* dest, const void* src, size_t size) {
#if 0
    uint8_t* d = (uint8_t*)dest;
    uint8_t* s = (uint8_t*)src;
    while (size--) {
        *d++ = *s++;
    }
#else
    uint16_t* d = (uint16_t*)dest;
    uint16_t* s = (uint16_t*)src;
    while (size >= 2) {
        *d++ = *s++;
        size -= 2;
    }
    if (size) {
        ((uint8_t&)*d) = *(uint8_t*)s;
    }
#endif
    return dest;
}

// TODO: implement assert
#define assert(cond)

template <size_t MaxSize>
class SimpleAllocator {
public:
    SimpleAllocator() { m_curr = m_buffer; }

    void* Allocate(size_t bytes) {
        void* block = m_curr;
        m_curr += bytes;
        assert(m_curr < (m_buffer + MaxSize));
        return block;
    }

    // template <typename T>
    // void* Allocate() {
    //     return Allocate(sizeof(T));
    // }

    // template <typename T>
    // T* New() {
    //     void* mem = Allocate(sizeof(T));
    //     return new (mem) T();
    // }

private:
    uint8_t m_buffer[MaxSize];
    uint8_t* m_curr;
};

#define FORCE_INLINE __attribute__((always_inline))

// Wrapper around Vectrex bios routines
namespace bios {
    const uint8_t DefaultIntensity = 127;
    const uint8_t DefaultScale = 200;

    // Call once at startup
    void Init();

    // Call once per frame, beam will be reset to 0,0
    void WaitFrame();

    // Resets beam to 0,0
    void ZeroBeam() { Reset0Ref(); }

    // Set intensity from 0 to 127 (brightest)
    void SetBeamIntensity(uint8_t intensity) { Intensity_a(intensity); }

    // Set scale from 0 to 127
    void SetScale(uint8_t scale) { VIA_t1_cnt_lo = scale; }

    // Draw vector list
    void Draw(const int8_t* const& vectorList, uint8_t relx = 0, uint8_t rely = 0,
              uint8_t scale = DefaultScale) {
        Moveto_d(relx, rely);
        SetScale(scale);
        Draw_VLc(vectorList);
    }
} // namespace bios

// Implementations
inline void bios::Init() {
    SetBeamIntensity(DefaultIntensity);
    SetScale(DefaultScale);
}

inline void bios::WaitFrame() {
    Wait_Recal();

    // Need to set beam intensity after Wait_Recal
    SetBeamIntensity(DefaultIntensity);

    // SetScale(DefaultScale);
}

typedef SimpleAllocator<60> SimpleAllocatorType;
SimpleAllocatorType g_simpleAllocaor;

// Structure of vector list data
struct VectorListData {
    VectorListData() {}

    static VectorListData& FromMemory(int8_t* volatile memory) {
        return *reinterpret_cast<VectorListData*>(memory);
    }

    // Create new VectorListData from array of (y,x) offsets
    static VectorListData* Create(int8_t* vertices, int8_t size) {
        VectorListData* data = (VectorListData*)g_simpleAllocaor.Allocate(size + 1);
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

int main() {
    int8_t paddleVertices[] = {
        3,       // size - 1
        0,   5,  //
        -20, 0,  //
        0,   -5, //
        20,  0   //
    };

    const int SIZE = 4;
    int8_t crossVertices[] = {
        4,                   //
        SIZE / 2,  SIZE / 2, //
        -SIZE,     -SIZE,    //
        SIZE / 2,  SIZE / 2, //
        -SIZE / 2, SIZE / 2, //
        SIZE,      -SIZE,    //
    };

    VectorList paddle(VectorListData::FromMemory(paddleVertices));
    // VectorList cross(VectorListData::FromMemory(crossVertices));
    VectorListData* crossData = VectorListData::Create(&crossVertices[1], 10);
    VectorList cross(*crossData);

    bios::Init();

    while (true) {
        bios::WaitFrame();

        paddle.Draw();
        cross.Draw();
    }

    return 0;
}
