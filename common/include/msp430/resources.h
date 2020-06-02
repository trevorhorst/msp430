#ifndef RESOURCES_H
#define RESOURCES_H

extern const char font[];
extern const unsigned int font_size;

const char *load( const char *resource, unsigned int size )
{
    char *r = 0; // new char[ size + 1 ];
    // memcpy( r, resource, size );
    // r[ size ] = '\0';
    return r;
}

char getChar( int offset )
{
    return font[ offset ];
}


/**
 * @brief Unloads a resource file
 * @param resource Resource file to unload
 */
void unload( const char *resource )
{
    if( resource ) {
        // delete[] resource;
    }
}


#endif // RESOURCES_H
