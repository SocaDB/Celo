#ifndef SOCKETHELP_H
#define SOCKETHELP_H

int listening_socket( const char *port ); ///< return a binded listening socket on port $port
int accepting_socket( int listen_fd ); ///< return socket after accept
int set_non_blocking( int sock ); ///<

#endif // SOCKETHELP_H
