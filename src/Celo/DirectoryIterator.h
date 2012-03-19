#ifndef DIRECTORYITERATOR_H
#define DIRECTORYITERATOR_H

/**
  @brief Can be used to sweep a directory

  Example :
\code
for( DirectoryIterator f( "/home" ); f; ++f )
    PRINT( *f );
\endcode
*/
struct DirectoryIterator {
    DirectoryIterator( const char *directory );
    ~DirectoryIterator();

    operator bool() const;
    DirectoryIterator &operator++();
    const char *operator*() const;

protected:
    void *dir;
    void *dir_ent;
};

bool is_a_directory( const char *n );



#endif // DIRECTORYITERATOR_H
