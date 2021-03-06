#pragma once

#define nullptr 0

#define FORCE_INLINE __attribute__((always_inline))

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

typedef unsigned int uint8_t;
typedef signed int int8_t;
typedef unsigned long int uint16_t;
typedef signed long int int16_t;
typedef uint16_t size_t;

static_assert(sizeof(uint8_t) == 1);
static_assert(sizeof(int8_t) == 1);
static_assert(sizeof(uint16_t) == 2);
static_assert(sizeof(int16_t) == 2);

namespace detail {
    void AssertHandler(const char* cond, const char* file, size_t line);
}
#define assert(cond)                                                                               \
    do {                                                                                           \
        if (!(cond))                                                                               \
            detail::AssertHandler(#cond, __FILE__, __LINE__);                                      \
    } while (false)

// Default placement new. Simply returns second argument.
inline void* operator new(size_t, void* mem) {
    return mem;
}

inline void* memcpy(void* dest, const void* src, size_t size) {
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

inline size_t strlen(const char* s) {
    size_t len = 0;
    while (*s++) {
        ++len;
    }
    return len;
}

inline char* itoa(int value, char* str) {
    int count = 0;
    int v = value;
    do {
        ++count;
        v /= 10;
    } while (v);
    v = value;
    str[count] = '\0';
    while (count) {
        str[count - 1] = '0' + v % 10;
        v /= 10;
        --count;
    }
    return str;
}

// Implement missing std stuff
namespace std {
    template <typename T>
    T min(T lhs, T rhs) {
        return lhs < rhs ? lhs : rhs;
    }

    template <typename T>
    T max(T lhs, T rhs) {
        return lhs > rhs ? lhs : rhs;
    }

    template <typename T, size_t sz>
    size_t size(T (&arr)[sz]) {
        return sz;
    }

    template <typename T>
    T abs(T v) {
        return v >= 0 ? v : -v;
    }
} // namespace std
