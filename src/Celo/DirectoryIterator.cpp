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

