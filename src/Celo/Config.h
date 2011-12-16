#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef int8_t      SI8 ;
typedef int16_t     SI16;
typedef int32_t     SI32;
typedef int64_t     SI64;

typedef uint8_t     PI8 ;
typedef uint16_t    PI16;
typedef uint32_t    PI32;
typedef uint64_t    PI64;

// ---------------------- size types ----------------------

namespace Level1 {
    template<int N> struct SigSChoice {};
    template<> struct SigSChoice<4> { typedef SI32 T; };
    template<> struct SigSChoice<8> { typedef SI64 T; };

    template<int N> struct PosSChoice {};
    template<> struct PosSChoice<4> { typedef PI32 T; };
    template<> struct PosSChoice<8> { typedef PI64 T; };
}

typedef Level1::SigSChoice<sizeof(void *)>::T ST; ///< signed size type
typedef Level1::PosSChoice<sizeof(void *)>::T PT; ///< unsigned size type


#endif // CONFIG_H
