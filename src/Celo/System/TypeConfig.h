/*
 Copyright 2012 Structure Computation  www.structure-computation.com
 Copyright 2012 Hugo Leclerc

 This file is part of Celo.

 Celo is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Celo is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with Celo. If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#ifdef METIL_COMP_DIRECTIVE
#pragma cpp_flag -std=c++11
#endif

namespace Celo {

typedef bool        Bool;

typedef int8_t      SI8 ;
typedef int16_t     SI16;
typedef int32_t     SI32;
typedef int64_t     SI64;

typedef uint8_t     PI8 ;
typedef uint16_t    PI16;
typedef uint32_t    PI32;
typedef uint64_t    PI64;

typedef float       FP32;
typedef double      FP64;
typedef long double FP80;

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

}

#endif // CONFIG_H
