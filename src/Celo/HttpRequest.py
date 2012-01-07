import os, sys, string

def nhex( name, litle_endian ):
    if litle_endian:
        return nhex( reversed( name ), 0 )
    r = "0x"
    for n in name:
        r += hex( ord( n ) )[ 2: ]
    return r

def cvar( n ):
    return n.replace( "_", "__" ).replace( " ", "_s" ).replace( "\n", "_n" ).replace( "\r", "_r" ).replace( ":", "_d" ).replace( "-", "_m" )

def cstr( n ):
    if n == "\n": return "\\n"
    if n == "\r": return "\\r"
    return n
    
def lookup_rec( f, labels, names, prefix, offset, proc_map, fallback ):
    crefix = cvar( prefix )
    labels.append( crefix )
    
    m = {}
    for i in names:
        if not len( i ):
            f.write( "a_" + crefix + ": " + proc_map[ prefix ][ 0 ] + "\n" )
            f.write( "b_" + crefix + ": " + proc_map[ prefix ][ 1 ] + "\n" )
            return
        if not ( i[ 0 ] in m ):
            m[ i[ 0 ] ] = []
        m[ i[ 0 ] ].append( i[ 1: ] )

    if len( prefix ):
        f.write( "a_" + crefix + ":\n" )
        f.write( "    ++data;\n" )
        f.write( "b_" + crefix + ":\n" )
        f.write( "    if ( data >= end ) goto c_" + crefix + ";\n" )
            
    f.write( "l_" + crefix + ":\n" )
    
    if offset >= 0 and offset % 4 == 0 and max( len( n ) for n in names ) >= 4:
        f.write( "    if ( end - data >= 4 ) {\n" )
        f.write( "        int val = *reinterpret_cast<const int *>( data );\n" )
        f.write( "\n" )
        f.write( "        #if __BYTE_ORDER == __LITTLE_ENDIAN\n" )
        for n in names:
            if len( n ) >= 4:
                f.write( "        if ( val == " + nhex( n[0:4], 1 ) + " ) { data += 4; goto b_" + prefix + cvar( n[0:4] ) + "; }\n" )
        f.write( "        #elif __BYTE_ORDER == __BIG_ENDIAN\n" )
        for n in names:
            if len( n ) >= 4:
                f.write( "        if ( val == " + nhex( n[0:4], 0 ) + " ) { data += 4; goto b_" + prefix + cvar( n[0:4] ) + "; }\n" )
        f.write( "        #else\n" )
        f.write( "        Unknown endianness\n" )
        f.write( "        #endif\n" )
        f.write( "    }\n\n" )
    
    if len( m ) > 3:
        f.write( "    switch ( *data ) {\n" )
        for key in m.keys():
            f.write( "    case '" + cstr( key ) + "': goto a_" + crefix + key + ";\n" )
        f.write( "    default: { " + fallback + " }\n" )
        f.write( "    }\n" )
    else:
        k = m.keys()
        for key in k[ 1 : ]:
            f.write( "    if ( *data == '" + cstr( key ) + "' ) goto a_" + crefix + key + ";\n" )
        f.write( "    if ( *data != '" + cstr( k[ 0 ] ) + "' ) { " + fallback + " }\n" )
        
    for key, val in m.items():
        lookup_rec( f, labels, val, prefix + key, offset + 1, proc_map, fallback )

def lookup( f, names, fallback, prefix = "", offset = -100000 ):
    labels = []

    proc_map = {}
    for name, procs in names.items():
        proc_map[ prefix + name ] = procs

    # l_...:
    lookup_rec( f, labels, names.keys(), prefix, offset, proc_map, fallback )
    
    # c_...:
    for l in labels:
        f.write( "c_" + l + ": inp_cont = &&l_" + l + "; return;\n" )
        
        
# req read
fl = file( "src/Celo/HttpRequest_read_req.h", "w" )
mr = {}
for n in [ "GET ", "POST ", "PUT ", "DELETE ", "TRACE ", "OPTIONS ", "CONNECT ", "HEAD " ]:
    mr[ n ] = [ " req_type = " + n[ : -1 ] + "; goto " + o + "_url_beg;" for o in "ab" ]
lookup( fl, mr, "goto e_400;", offset = 0 )

# header_fields
fl = file( "src/Celo/HttpRequest_header_fields.h", "w" )
mr = {
    "\nContent-Length: " : [ "goto a_read_cl;", "goto b_read_cl;" ],
    "\n\n" : [ "goto l_user;", "goto l_user;" ],
    "\n\r\n" : [ "goto l_user;", "goto l_user;" ]
}
lookup( fl, mr, "goto l_header_fields_;", prefix = "header_fields_" )


# conts
l = string.split( "url_beg url_cnt read_cl" )

f = file( "src/Celo/HttpRequest_conts.h", "w" )
for i in l:
    f.write( "c_" + i + ": inp_cont = &&l_" + i + "; return;\n" )


