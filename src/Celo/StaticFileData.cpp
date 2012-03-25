#include "DirectoryIterator.h"
#include "StaticFileData.h"
#include "StringHelp.h"
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

static bool ends_with( const string &str, const string &end ) {
    return str.size() >= end.size() and str.substr( str.size() - end.size() ) == end;
}

static int size_file( const string &filename ) {
    ifstream fi( filename.c_str() );
    if ( not fi )
        return 0;

    // get size
    fi.seekg( 0, ios_base::end );
    return fi.tellg();
}

static bool read_file( string &res, const string &filename ) {
    ifstream fi( filename.c_str() );
    if ( not fi )
        return false;

    // get size
    fi.seekg( 0, ios_base::end );
    size_t size = fi.tellg();
    fi.seekg( 0, ios_base::beg );

    // read
    char *tmp = new char[ size ];
    fi.read( tmp, size );

    // ret
    res += string( tmp, tmp + size );
    delete [] tmp;
    return true;
}

template<class T>
static string to_string( const T &val ) {
    ostringstream ss;
    ss << val;
    return ss.str();
}

struct DecreasingLength {
    bool operator()( const StaticFileData::File &a, const StaticFileData::File &b ) {
        return a.url.size() > b.url.size();
    }
};



StaticFileData::StaticFileData() {
    index_html_name = "index.html";
}

void StaticFileData::exec( const std::string &base ) {
    _fill_rec( base, "/" );
    sort( info.begin(), info.end(), DecreasingLength() );
}

bool StaticFileData::excluded( const std::string &f ) const {
    return f.size() == 0 or
           f[ 0 ] == '.' or
           f[ f.size() - 1 ] == '~';
}

const char *StaticFileData::mime_type( const std::string &url ) const {
    #define SH( E, R ) if ( ends_with( url, E ) ) return R;
    #include "MimeTypes.h"
    #undef SH
    return "text/plain";
}

void StaticFileData::_fill_rec( const std::string &base, const std::string &url_base ) {
    for( DirectoryIterator i( base.c_str() ); i; ++i ) {
        // exclusions
        if ( excluded( *i ) )
            continue;

        // recursion
        std::string n = base + '/' + *i;
        if ( is_a_directory( n.c_str() ) ) {
            _fill_rec( n, url_base + *i + '/' );
            continue;
        }

        // get data
        File f;
        f.url = url_base + url_encode( *i );
        f.offset = data.size();

        data += "HTTP/1.0 200 OK\n";
        data += "Content-Type: " + string( mime_type( n ) ) + "\n";
        data += "Content-Length: " + to_string( size_file( n ) ) + "\n";
        data += "\n";
        read_file( data, n );

        // finalize
        f.length = data.size() - f.offset;
        info.push_back( f );

        // if n == toto/index.html, add an info for toto/ and toto
        if ( index_html_name == *i ) {
            // toto/
            f.url = url_base;
            info.push_back( f );

            // toto
            if ( url_base.size() > 1 and url_base[ url_base.size() - 1 ] == '/' ) {
                f.url = url_base.substr( 0, url_base.size() - 1 );
                info.push_back( f );
            }
        }
    }

}
