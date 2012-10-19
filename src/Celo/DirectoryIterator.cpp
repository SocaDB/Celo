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


#include "DirectoryIterator.h"
#include <sys/stat.h>
#include <dirent.h>

DirectoryIterator::DirectoryIterator( const char *directory ) {
    dir = (void *)opendir( directory );
    dir_ent = dir ? (void *)readdir( (DIR *)dir ) : 0;
}

DirectoryIterator::~DirectoryIterator() {
    if ( dir )
        closedir( (DIR *)dir );
}

DirectoryIterator::operator bool() const {
    return dir_ent;
}

DirectoryIterator &DirectoryIterator::operator++() {
    dir_ent = dir ? (void *)readdir( (DIR *)dir ) : 0;
    return *this;
}

const char *DirectoryIterator::operator*() const {
    return ((dirent *)dir_ent)->d_name;
}

bool is_a_directory( const char *n ) {
    struct stat stat_file;
    if ( stat( n, &stat_file ) )
        return false;
    return S_ISDIR( stat_file.st_mode );
}

