#include "StaticFileData.h"
#include "StringHelp.h"
#include "StreamSep.h"
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int usage( const char *prg, const char *msg, int res ) {
    StreamSepMaker<ostream> c( cerr );
    if ( msg )
        c << msg;
    c << "Usage:";
    c << "  " << prg << " [ options ]";
    c << "Possible options are:";
    c << "  -o file_name: file name for library output (dl_req.so by default)";
    c << "  -d directory: directory of files that has to be served statically (html by default)";
    c << "  -s sipe_file: sipe file for additionnal definition and surdefinitions";
    c << "  --sipe : path to the sipe executable";
    return res;
}

static void txt_to_cpp( ostream &fo, const string &fi ) {
    fo << "char _data[] = {";
    for( int i = 0; i < fi.size(); ++i ) {
        int c = fi[ i ];

        if ( i )
            fo << ", ";
        if ( i % 16 == 0 )
            fo << "\n    ";
        fo << c;

        if ( i + 1 == fi.size() ) {
            if ( i )
                fo << "\n";
            break;
        }
    }
    fo << "};\n";
}

int main( int argc, char **argv ) {
    string output = "dl_req";
    string base_d = "html";
    string sipe_e;

    for( int i = 1; i < argc; ++i ) {
        string arg = argv[ i ];
        #define RA( A, V ) \
            if ( arg == A ) { \
                if ( i + 1 == argc ) \
                    return usage( argv[ 0 ], A " must be followed by an argument", 1 ); \
                V = argv[ ++i ]; \
                continue; \
            }
        RA( "-o", output );
        RA( "-f", base_d );
        RA( "--sipe", sipe_e );
        return usage( argv[ 0 ], "", 2 );
    }

    StaticFileData sd;
    sd.exec( base_d );

    // sipe data
    string sipe_file = output + ".sipe";
    ofstream sipe( sipe_file.c_str() );
    StreamSepMaker<ostream> s( sipe );

    s << "additionnal_gurl = none";
    s << "gurl =";
    for( int i = 0; i < sd.info.size(); ++i )
        sipe << "    ( '" << sd.info[ i ].url << " ' { send_cst( _data + "
             << sd.info[ i ].offset << ", "
             << sd.info[ i ].length << " ); } ) |";
    s << "    additionnal_gurl";
    s << "get = 'GET ' gurl";
    s << "main = get | { error_400(); }";
    sipe.close();

    // sipe execution
    vector<string> sipe_v;
    if ( sipe_e.size() )
        sipe_v.push_back( sipe_e );
    else {
        sipe_v.push_back( "sipe" );
        sipe_v.push_back( "Sipe/sipe" );
        sipe_v.push_back( "../Sipe/sipe" );
    }

    string gen_file = output + "_parser.h";
    for( int i = 0; i < sipe_v.size(); ++i ) {
        if ( system( ( "which " + sipe_v[ i ] + " > /dev/null" ).c_str() ) == 0 ) {
            string cmd = sipe_v[ i ] + " -o " + gen_file + " " + sipe_file;
            if ( int ret = system( cmd.c_str() ) )
                return ret;
            break;
        }
    }

    // cpp data
    string cpp_file = output + ".cpp";
    ofstream cpp( cpp_file.c_str() );
    StreamSepMaker<ostream> c( cpp );
    c << "#include <Celo/EventObj_HttpRequest.h>";
    c << "#include <Celo/Listener_Factory.h>";
    c << "";
    txt_to_cpp( cpp, sd.data );
    c << "";
    c << "class _HttpRequest : public EventObj_HttpRequest {";
    c << "public:";
    c << "    _HttpRequest( int fd, void * ) : EventObj_HttpRequest( fd ) {}";
    c << "";
    c << "    #define SIPE_CHARP char *";
    c << "    #define SIPE_CLASS";
    c << "    #include <" << gen_file << ">";
    c << "};";
    c << "";
    c << "extern \"C\" Listener_WithLaunch *factory( const char *port ) {";
    c << "    return new Listener_Factory<_HttpRequest>( port, 0 );";
    c << "}";
    cpp.close();

    // cpp compilation
    string cmd = "metil_comp -no-env -I. -ICelo/src -O3 -dylib -o " + output + " " + cpp_file;
    // PRINT( cmd );
    return system( cmd.c_str() );
}
