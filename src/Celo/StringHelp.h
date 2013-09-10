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


#ifndef STRINGHELP_H
#define STRINGHELP_H

#include <iostream>
#include <assert.h>

#ifndef PRINT
    #define PRINT( A ) \
        std::cout << "  " << #A << " -> " << (A) << std::endl
    #define PRINTN( A ) \
        std::cout << "  " << #A << " ->\n" << (A) << std::endl
    #define TODO \
        std::cerr << "TODO " << __FILE__ << ":" << __LINE__ << std::endl
#endif

inline bool is_a_number( char a ) { return a >= '0' and a <= '9'; }
std::string url_encode( const std::string &str );
std::string url_decode( const std::string &str );


#endif // STRINGHELP_H
