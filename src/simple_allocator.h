#pragma once

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
