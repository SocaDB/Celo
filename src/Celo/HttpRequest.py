#from operator import attrgetter
#import os, sys

#class Op:
    #def __init__( self, string, action, probab = 1 ):
        #self.string = string
        #self.action = action
        #self.probab = probab
        #self.nextop = []
        #self.num_op = -1

    #def sortop( self ):
        #self.nextop.sort( key = attrgetter('probab'), reverse = True )

#def singlify_chars( c ):
    #global cur_op
    #if c.num_op == cur_op:
        #return
        
    #if len( c.string ) > 1:
        #t = Op( c.string[ 1: ], c.action )
        #c.string = c.string[ 0 ]
        #c.action = ""
        #c.nextop = [ t ]
        #return singlify_chars( t )
    #for n in c.nextop:
        #singlify_chars( n )

#def commonify_chars( c ):
    #global cur_op
    #if c.num_op == cur_op:
        #return

    #letters = {}
    #for n in c.nextop:
        #if n.string in letters:
            #letters[ n.string ] += 1
        #else:
            #letters[ n.string ] = 1
    ##print letters
    #nextop = []
    #for letter, number in letters.items():
        ##print letter, number
        #t = Op( letter, "" )
        #nextop.append( t )
        #t.probab = 0
        #for n in c.nextop:
            #if n.string == letter:
                #t.probab += n.probab
                #t.nextop.extend( n.nextop )
    #c.nextop = nextop
    

#def set_num( c ):
    #global cur_op
    #if c.num_op == cur_op:
        #return
        
    #global num_op
    #c.num = num_op
    #num_op += 1
    #for n in c.nextop:
        #set_num( n )
    
#def write_dot( f, op ):
    #f.write( '    node' + str( op.num ) + ' [label="' + op.string + '"];\n' )
    #for n in op.nextop:
        #f.write( '    node' + str( op.num ) + ' -> node' + str( n.num ) + ';\n' )
        #write_dot( f, n )
       
#def display_dot( f, op ):
    #f.write( "digraph popo {\n" )
    #write_dot( f, op )
    #f.write( "}\n" )

#def write_code( f, c ):
    #f.write( "label_" + str( c.num ) + "V:\n" )
    #f.write( "    if ( ++data >= end ) return READ_REQ_" + str( c.num ) + "\n" )
    #f.write( "label_" + str( c.num ) + ":\n" )
    #if len( c.action ):
        #f.write( "    { " + c.action + " }\n" )
    #c.sortop()
    #for n in c.nextop:
        #f.write( "    if ( *data == '" + n.string + "' ) goto label_" + str( n.num ) + "V;\n" )
    #f.write( "    ERROR();\n" )
    #for n in c.nextop:
        #write_code( f, n )

#root = Op( "", "" )
#beg  = Op( "BEG " , "req_type = BEG;" , 3 )
#put  = Op( "PUT " , "req_type = PUT;" , 1 )
#post = Op( "POST ", "req_type = POST;", 2 )
##reqe = Op( "POST ", "req_type = POST;", 2 )
#root.nextop = [ beg, put, post ]

#cur_op = 0
#singlify_chars( root )
#commonify_chars( root )

#num_op = 0
#set_num( root )


##display_dot( file( ".res.dot", "w" ), root )
##os.system( "dot -Tps .res.dot > .res.ps && gv .res.ps" )

#write_code( sys.stdout, root )
n = 41

f = file( "src/Celo/HttpRequest_jumps.h", "w" )
f.write( "switch ( cur_proc ) {\n" )
for i in range( n ):
    f.write( "    case " + str( i ) + ": goto l_" + str( i ) + ";\n" )
f.write( "}\n" )

f = file( "src/Celo/HttpRequest_conts.h", "w" )
for i in range( n ):
    f.write( "c_" + str( i ) + ": cur_proc = " + str( i ) + "; return false;\n" )


