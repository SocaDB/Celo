#ifndef STATICFILEDATA_H
#define STATICFILEDATA_H

#include "StringHelp.h"
#include <vector>

/**

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

    virtual bool excluded( const std::string &filename ) const; ///< return
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
