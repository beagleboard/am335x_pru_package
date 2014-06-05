/*
 * path_utils.c
 *
 * Copyright (C) 2014 Spencer E Olson
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/*===========================================================================
 * Copyright (c) Spencer E Olson 2014
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied or provided.
 * ============================================================================
 */

/*===========================================================================
// PASM - PRU Assembler
//---------------------------------------------------------------------------
//
// File     : path_utils.c
//
// Description:
//     File path utilities to make things easier...
//
============================================================================*/

#include "path_utils.h"

#include <string.h>
#ifdef _MSC_VER
#include <io.h>
#define access _access
#define R_OK 4
#else
#include <unistd.h> // for access(...)
#endif
#include <stdlib.h>   // for _splitpath, _makepath, malloc/realloc

#ifdef _UNIX_
  #include <libgen.h>
#else // Assume windows (?)
  #include <Shlwapi.h>  // for PathIsRelative
#endif


static char ** include_dirs = NULL;
static size_t num_include_dirs = 0;

int add_include_dir( const char * dirname )
{
    // allocate and initialize new memory for storing the new directory
    char * new_dir = (char *)malloc( strlen(dirname) + 1 );
    if ( new_dir == NULL )
        return -1;
    strcpy( new_dir, dirname );

    // Strip away trailing '/'
    while ( new_dir[ strlen(new_dir) -1 ] == '/' )
      new_dir[ strlen(new_dir) -1 ] = '\0';

    // increase memory for the array of directories by one and store dir
    ++num_include_dirs;
    include_dirs=(char**)realloc(include_dirs, sizeof(char**)*num_include_dirs);
    include_dirs[ num_include_dirs - 1 ] = new_dir;
    return 0;
}


int get_absolute( char * filename, const size_t sz )
{
    int retval = -1;
    size_t i, dlen = 0, flen = strnlen(filename, sz-1);
    char * scratch = (char *) malloc( sz );
    for (i = 0; i < num_include_dirs; ++i )
    {
        dlen = strlen(include_dirs[i]);
        if ( (dlen + flen) >= sz )
        {
            retval = 1;
            break;
        }
        strcpy( scratch, include_dirs[i] );
        scratch[ dlen ] = '/'; // TODO:  test on windows if mixed with '\\'.
        strcpy( scratch+dlen+1, filename ); // this should terminate it

        if ( access( scratch, R_OK ) == 0 )
        {
            strcpy( filename, scratch );
            retval = 0;
            break;
        }
    }

    free(scratch);
    return retval;
}


int is_definite( const char * filename )
{
    #ifdef _UNIX_
    return (filename[0] == '/') || (filename[0] == '.');

    #else // Should we assume windows?!
    // Windows impl
    return (PathIsRelative( filename ) == FALSE) || (filename[0] == '.');
    #endif
}



int get_dirname( const char * filename, char * dir, const size_t sz)
{
    #ifdef _UNIX_
    char * p = NULL;
    #else
    char drive[_MAX_DRIVE];
    char dir_only[_MAX_DIR];
    #endif
    size_t f_len = strlen(filename);
    if ( f_len >= sz )
        return -1;

    #ifdef _UNIX_
    // to allow for dirname to change contents of arg
    strcpy( dir, filename );
    p = dirname( dir );
    if (p == NULL)
      return -1;
    if (dir != p)
        strcpy( dir, p );

    #else // assume windows(?)
    _splitpath( filename, drive, dir_only, NULL, NULL );
    /*------------------------------------------------------------------------
    LJN: POSIX dirname() returns a . when the path is empty
    ------------------------------------------------------------------------*/
    if( strlen( dir_only ) == 0 )
        {
        strcpy( dir_only, "." );
        }
    _makepath( dir, drive, dir_only, NULL, NULL );

    /*------------------------------------------------------------------------
    LJN: POSIX dirname() does not end the path with a / or \
    ------------------------------------------------------------------------*/
    if( ( dir[ strlen( dir ) - 1 ] == '/'  )
     || ( dir[ strlen( dir ) - 1 ] == '\\' ) )
        {
        dir[ strlen( dir ) - 1 ] = '\0';
        }
    #endif

    return 0;
}

int get_basename( const char * filename, char * base, const size_t sz)
{
    #ifdef _UNIX_
    char * tmp;
    char * p;
    #else // assume windows(?)
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    #endif
    
    size_t f_len = strlen(filename);

    #ifdef _UNIX_
    // to allow for basename to change contents of arg
    tmp = (char*) malloc( f_len+1 );
    strcpy( tmp, filename );
    p = basename( tmp );
    if (p == NULL || strlen(p) >= sz)
    {
        free(tmp);
        return -1;
    }
    strcpy( base, p );
    free(tmp);

    #else // assume windows(?)
    _splitpath( filename, NULL, NULL, fname, ext );
    _makepath( base, NULL, NULL, fname, ext );
    #endif

    return 0;
}
