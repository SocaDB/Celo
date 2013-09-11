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


#ifndef STATICFILEDATA_H
#define STATICFILEDATA_H

#include "../Util/StringHelp.h"
#include <vector>

/**
  Permit to construct (in this->data and in this->info) info that contains html/js/... pages in memory with included headers
*/
class StaticFileData {
public:
    struct File {
        std::string url;
        int offset; ///< offset in data
        int length; ///< length of data to send
    };

    StaticFileData();
    void exec( const std::string &base );

    virtual bool excluded( const std::string &filename ) const; ///< return true if a filename must be excluded
    virtual const char *mime_type( const std::string &url ) const; ///< return e.g. text/html for a .html file

    // input
    std::string index_html_name; ///< "index.html" by default

    // output
    std::vector<File> info; ///< sorted by file length (decreasing order)
    std::string data; ///< file data

protected:
    void _fill_rec( const std::string &base, const std::string &url_base );
};

#endif // STATICFILEDATA_H
