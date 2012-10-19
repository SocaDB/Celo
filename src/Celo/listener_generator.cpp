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


#include "StaticFileData.h"
#include "InstallDir.h"
#include "StringHelp.h"
#include "StreamSep.h"
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

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

static void append( std::ostream &os, const char *inp ) {
    std::ifstream is( inp );
    while ( true ) {
        char buffer[ 2048 ];
        int r = is.readsome( buffer, sizeof buffer );
        if ( r <= 0 )
            break;
        std::cout.write( buffer, r );
        os.write( buffer, r );
    }
}


int usage( const char *prg, const char *msg, int res ) {
    StreamSepMaker<ostream> c( cerr );
    if ( msg )
        c << msg;
    c << "Usage:";
    c << "  " << prg << " [ options ]";
    c << "Possible options are:";
    #define RA( A, V, I, D ) c << "  " << A << " " << I << " (default value='" << D << "'";
    #include "listener_generator_args.h"
    #undef RA
    return res;
}

int main( int argc, char **argv ) {
    #define RA( A, V, I, D ) string V = D;
    #include "listener_generator_args.h"
    #undef RA

    for( int i = 1; i < argc; ++i ) {
        string arg = argv[ i ];
        #define RA( A, V, I, D ) \
            if ( arg == A ) { \
                if ( i + 1 == argc ) \
                    return usage( argv[ 0 ], A " must be followed by an argument", 1 ); \
                V = argv[ ++i ]; \
                continue; \
            }
        #include "listener_generator_args.h"
        #undef RA
        return usage( argv[ 0 ], "", 2 );
    }

    StaticFileData sd;
    sd.exec( base_dir );

    // sipe data
    string sipe_file = output + ".sipe";
    ofstream sipe( sipe_file.c_str() );
    StreamSepMaker<ostream> s( sipe );

    s << "additionnal_gurl = none";
    s << "additionnal_purl = none";
    s << "gurl =";
    // PRINT(\"requesting " << sd.info[ i ].url << "\");
    for( int i = 0; i < sd.info.size(); ++i )
        s << "    ( '" << sd.info[ i ].url << " ' { send_cst( _data + "
          << sd.info[ i ].offset << ", "
          << sd.info[ i ].length << " ); } ) |";
    s << "    additionnal_gurl";

    s << "get  = 'GET ' gurl";
    s << "post = 'POST ' additionnal_purl";
    s << "main = get | post | { error_400(); }";

    if ( sipe_ext.size() )
        append( sipe, sipe_ext.c_str() );
    sipe.close();


    // sipe execution
    vector<string> sipe_v;
    if ( sipe_exe.size() )
        sipe_v.push_back( sipe_exe );
    else {
        sipe_v.push_back( "sipe" );
        sipe_v.push_back( INSTALL_DIR "/Sipe/sipe" );
        sipe_v.push_back( "../Sipe/sipe" );
    }

    string gen_file = output + "_parser.h";
    for( int i = 0; ; ++i ) {
        if ( i == sipe_v.size() ) {
            cerr << "In " << argv[ 0 ] << ", the 'sipe' executable not found (the -s flag can be usefull).\n";
            return 3;
        }
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
    if ( base_str == "EventObj_HttpRequest" )
        c << "#include <Celo/EventObj_HttpRequest.h>";
    else
        c << "#include <" << base_str << ".h>";
    c << "#include <Celo/Listener_Factory.h>";
    c << "#include <Celo/StringHelp.h>";
    c << "";
    c << "";
    txt_to_cpp( cpp, sd.data );
    c << "";
    c << "class _HttpRequest : public " << base_str << " {";
    c << "public:";
    c << "    _HttpRequest( int fd, void *data ) : " << base_str << "( fd, data ) {}";
    c << "";
    c << "    #define SIPE_CHARP char *";
    c << "    #define SIPE_CLASS";
    c << "    #include <" << gen_file << ">";
    c << "};";
    c << "";
    c << "extern \"C\" Listener_WithLaunch *factory( const char *port, void *data ) {";
    c << "    return new Listener_Factory<_HttpRequest>( port, data );";
    c << "}";
    cpp.close();

    // cpp compilation
    string cmd = "metil_comp -no-env -I. -I" INSTALL_DIR "/src -O3 -dylib -o " + output;
    if ( inc_path.size() )
        cmd += " -I" + inc_path;
    cmd += " " + cpp_file;
    PRINT( cmd );
    return system( cmd.c_str() );
}
