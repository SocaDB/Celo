import os, sys, string, operator

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

class Out:
    def __init__( self, f ):
        self.f = f
        self.l = []
        
    def __iadd__( self, line ):
        self.f.write( line + "\n" )
        return self
        
    def add_cnt( self, c ):
        self.l.append( c )
    
    def finalize( self ):
        for c in self.l:
            self += "c_" + c + ": inp_cont = &&l_" + c + "; return;"
 
class Word:
    def __init__( self, word, prob = 1 ):
        self.word = word
        self.prob = prob # unnormalized probability

        
class ParseItem:
    nb_prefix = 0
    
    def __init__( self ):
        self.next = None
        
    def new_prefix( self ):
        ParseItem.nb_prefix += 1
        return str( ParseItem.nb_prefix ) + "_"
        
class Choice( ParseItem ):
    def __init__( self ):
        self.choices = []
        self.fallback = ""
        
    def __iadd__( self, word ):
        self.choices.append( w )
        return self
    
    def write( self, f ):
        self._write_rec( f, self.new_prefix(), "" )
        
    def _fallback( self, prefix, beg ):
        if len( self.fallback ):
            return self.fallback
        return "goto l_" + prefix + ";"
        
    def _write_rec( self, f, prefix, beg ):
        f += "l_" + prefix + cvar( beg ) + ":"

        for c in self.choices:
            if c.word == beg:
                f += "    // " + beg
                if c.next != None:
                    c.next.write( f )
                return

        m = {}
        for c in self.choices:
            if len( c.word ) > len( beg ) and c.word.startswith( beg ):
                l = c.word[ len( beg ) ]
                if l in m: m[ l ] += c.prob
                else:      m[ l ] = c.prob
        k = [ p[ 0 ] for p in sorted( m.iteritems(), key = operator.itemgetter( 1 ), reverse = True ) ]

        for p in reversed( k[ 1: ] ):
            l = prefix + cvar( beg + p )
            out.add_cnt( l )
            f += "    if ( *data == '" + p + "' ) { if ( ++data >= end ) goto c_" + l + "; goto l_" + l + "; }"
        f += "    if ( *data != '" + k[ 0 ] + "' ) " + self._fallback( beg, prefix )
        l = prefix + cvar( beg + k[ 0 ] )
        out.add_cnt( l )
        f += "    if ( ++data >= end ) goto c_" + l + ";"
 
 
        for p in k:
            self._write_rec( f, prefix, beg + p )
            
 
class String( ParseItem ):
    def __init__( self, varname ):
        self.varname = varname
        self.end = ' '
    
    def write( self, f ):
        l = self.new_prefix()
        f += """
a_{l}_beg:
    ++data;
b_{l}_beg:
    if ( data >= end ) goto c_{l}_cnt;
l_{l}_beg: // first call
    {varname}_data = data;
    while ( true ) {
        if ( *data == ' ' ) {
            {varname}_size = data - {varname}_data;
            *data = 0;
            goto e_{l};
        }
        if ( ++data == end ) {
            const char *old = {varname}_data;
            {varname}_size = data - {varname}_data;
            {varname}_rese = 2 * size_buff;
            {varname}_data = (char *)malloc( {varname}_rese );
            memcpy( {varname}_data, old, {varname}_size );
            goto c_{l}_cnt;
        }
    }


l_{l}_cnt: // cont
    while ( true ) {
        if ( {varname}_size == {varname}_rese ) {
            char *old = {varname}_data;
            {varname}_data = (char *)malloc( {varname}_rese *= 2 );
            memcpy( {varname}_data, old, {varname}_size );
            free( old );
        }

        if ( *data == ' ' ) {
            {varname}_data[ {varname}_size ] = 0;
            goto e_{l};
        }
        {varname}_data[ {varname}_size++ ] = *data;

        if ( ++data == end )
            goto c_{l}_cnt;
    }

e_{l}:
    if ( req_type == GET ) {
        inp_cont = 0;
        return req();
    }
""".replace( "{l}", l )

choice = Choice()
choice.fallback = "goto e_400;"
for n in [ ( "GET ", 5 ), ( "POST ", 3 ), ( "PUT ", 1 ) ]: # , "DELETE ", "TRACE ", "OPTIONS ", "CONNECT ", "HEAD "
    w = Word( n[ 0 ], n[ 1 ] )
    choice += w
    
    s = String()
    w.next = s

out = Out( sys.stdout )
choice.write( out )
out.finalize()
 
#def lookup_rec( f, labels, names, prefix, offset, proc_map, fallback ):
    #crefix = cvar( prefix )
    #labels.append( crefix )
    
    #m = {}
    #for i in names:
        #if not len( i ):
            #f.write( "a_" + crefix + ": " + proc_map[ prefix ][ 0 ] + "\n" )
            #f.write( "b_" + crefix + ": " + proc_map[ prefix ][ 1 ] + "\n" )
            #return
        #if not ( i[ 0 ] in m ):
            #m[ i[ 0 ] ] = []
        #m[ i[ 0 ] ].append( i[ 1: ] )

    #if len( prefix ):
        #f.write( "a_" + crefix + ":\n" )
        #f.write( "    ++data;\n" )
        #f.write( "b_" + crefix + ":\n" )
        #f.write( "    if ( data >= end ) goto c_" + crefix + ";\n" )
            
    #f.write( "l_" + crefix + ":\n" )
    
    #if offset >= 0 and offset % 4 == 0 and max( len( n ) for n in names ) >= 4:
        #f.write( "    if ( end - data >= 4 ) {\n" )
        #f.write( "        int val = *reinterpret_cast<const int *>( data );\n" )
        #f.write( "\n" )
        #f.write( "        #if __BYTE_ORDER == __LITTLE_ENDIAN\n" )
        #for n in names:
            #if len( n ) >= 4:
                #f.write( "        if ( val == " + nhex( n[0:4], 1 ) + " ) { data += 4; goto b_" + prefix + cvar( n[0:4] ) + "; }\n" )
        #f.write( "        #elif __BYTE_ORDER == __BIG_ENDIAN\n" )
        #for n in names:
            #if len( n ) >= 4:
                #f.write( "        if ( val == " + nhex( n[0:4], 0 ) + " ) { data += 4; goto b_" + prefix + cvar( n[0:4] ) + "; }\n" )
        #f.write( "        #else\n" )
        #f.write( "        Unknown endianness\n" )
        #f.write( "        #endif\n" )
        #f.write( "    }\n\n" )
    
    #if len( m ) > 3:
        #f.write( "    switch ( *data ) {\n" )
        #for key in m.keys():
            #f.write( "    case '" + cstr( key ) + "': goto a_" + crefix + key + ";\n" )
        #f.write( "    default: { " + fallback + " }\n" )
        #f.write( "    }\n" )
    #else:
        #k = m.keys()
        #for key in k[ 1 : ]:
            #f.write( "    if ( *data == '" + cstr( key ) + "' ) goto a_" + crefix + key + ";\n" )
        #f.write( "    if ( *data != '" + cstr( k[ 0 ] ) + "' ) { " + fallback + " }\n" )
        
    #for key, val in m.items():
        #lookup_rec( f, labels, val, prefix + key, offset + 1, proc_map, fallback )

#def lookup( f, names, fallback, prefix = "", offset = -100000 ):
    #labels = []

    #proc_map = {}
    #for name, procs in names.items():
        #proc_map[ prefix + name ] = procs

    ## l_...:
    #lookup_rec( f, labels, names.keys(), prefix, offset, proc_map, fallback )
    
    ## c_...:
    #for l in labels:
        #f.write( "c_" + l + ": inp_cont = &&l_" + l + "; return;\n" )
        
        
## req read
#fl = file( "src/Celo/HttpRequest_read_req.h", "w" )
#mr = {}
#for n in [ "GET ", "POST ", "PUT ", "DELETE ", "TRACE ", "OPTIONS ", "CONNECT ", "HEAD " ]:
    #mr[ n ] = [ " req_type = " + n[ : -1 ] + "; goto " + o + "_{l};" for o in "ab" ]
#lookup( fl, mr, "goto e_400;", offset = 0 )

## header_fields
#fl = file( "src/Celo/HttpRequest_header_fields.h", "w" )
#mr = {
    #"\nContent-Length: " : [ "goto a_read_cl;", "goto b_read_cl;" ],
    #"\n\n" : [ "goto l_user;", "goto l_user;" ],
    #"\n\r\n" : [ "goto l_user;", "goto l_user;" ]
#}
#lookup( fl, mr, "goto l_header_fields_;", prefix = "header_fields_" )


## conts
#l = string.split( "{l} url_cnt read_cl" )

#f = file( "src/Celo/HttpRequest_conts.h", "w" )
#for i in l:
    #f.write( "c_" + i + ": inp_cont = &&l_" + i + "; return;\n" )


