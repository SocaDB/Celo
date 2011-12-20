import os, sys

class Op:
    def __init__( self, string, action ):
        self.string = string
        self.action = action
        self.nextop = []

def singlify_chars( c ):
    if len( c.string ) > 1:
        t = Op( c.string[ 1: ], c.action )
        c.string = c.string[ 0 ]
        c.action = ""
        c.nextop = [ t ]
        return singlify_chars( t )
    for n in c.nextop:
        singlify_chars( n )

def commonify_chars( c ):
    letters = {}
    for n in c.nextop:
        if n.string in letters:
            letters[ n.string ] += 1
        else:
            letters[ n.string ] = 1
    #print letters
    nextop = []
    for letter, number in letters.items():
        #print letter, number
        t = Op( letter, "" )
        nextop.append( t )
        for n in c.nextop:
            if n.string == letter:
                t.nextop.extend( n.nextop )
    c.nextop = nextop
    

def set_num( op ):
    global num_op
    op.num = num_op
    num_op += 1
    for n in op.nextop:
        set_num( n )
    
def write_dot( f, op ):
    f.write( '    node' + str( op.num ) + ' [label="' + op.string + '"];\n' )
    for n in op.nextop:
        f.write( '    node' + str( op.num ) + ' -> node' + str( n.num ) + ';\n' )
        write_dot( f, n )
       
def display_dot( f, op ):
    f.write( "digraph popo {\n" )
    write_dot( f, op )
    f.write( "}\n" )

def write_code( f, op ):
    #f.write( "    if ( *data == '" + op.string + "' )" )

root = Op( "", "" )
beg  = Op( "BEG " , "req_type = BEG;"  )
put  = Op( "PUT " , "req_type = PUT;"  )
post = Op( "POST ", "req_type = POST;" )
root.nextop = [ beg, put, post ]

singlify_chars( root )
commonify_chars( root )

num_op = 0
set_num( root )


#display_dot( file( ".res.dot", "w" ), root )
#os.system( "dot -Tps .res.dot > .res.ps && gv .res.ps" )

write_code( sys.stdout, root )
