#include "../path_utils.h"

#include <stdio.h>
#include <string.h>

#define LOG(FORMAT, ...) fprintf(stderr, FORMAT, ## __VA_ARGS__)


int test_is_definite()
{
    int errors = 0;
    #ifdef _UNIX_
    if ( ! is_definite( "/hi/there" ) )
    {
        ++errors;
        LOG("is_definite fail with absolute path '/hi/there'\n");
    }
    #else //windows
    if ( ! is_definite( "c:/hi/there" ) )
    {
        ++errors;
        LOG("is_definite fail with absolute path 'c:/hi/there'\n");
    }

    if ( ! is_definite( "c:\\hi\\there" ) )
    {
        ++errors;
        LOG("is_definite fail with absolute path 'c:\\hi\\there'\n");
    }
    #endif

    if ( ! is_definite( "./hi/there" ) )
    {
        ++errors;
        LOG("is_definite fail with explicit relative path './hi/there'\n");
    }

    if ( ! is_definite( "../hi/there" ) )
    {
        ++errors;
        LOG("is_definite fail with explicit relative path '../hi/there'\n");
    }

    if ( is_definite( "hi/there" ) )
    {
        ++errors;
        LOG("is_definite fail with relative path 'hi/there'\n");
    }
    return errors;
}



int test_get_dirname()
{
    static const size_t sz = 256;
    int errors = 0;
    char scratch[sz];

    #ifdef _UNIX_
    scratch[0] = '\0';
    if ( get_dirname( "/hi/there", scratch, sz ) || strcmp("/hi", scratch) )
    {
        ++errors;
        LOG("get_dirname fail with absolute path '/hi/there' ['%s']\n", scratch);
    }
    #else //windows
    scratch[0] = '\0';
    if ( get_dirname( "c:/hi/there", scratch, sz ) || strcmp("c:/hi", scratch) )
    {
        ++errors;
        LOG("get_dirname fail with absolute path 'c:/hi/there ['%s']'\n", scratch);
    }

    scratch[0] = '\0';
    if ( get_dirname( "c:\\hi\\there", scratch, sz ) || strcmp("c:\\hi", scratch) )
    {
        ++errors;
        LOG("get_dirname fail with absolute path 'c:\\hi\\there ['%s']'\n", scratch);
    }
    #endif

    scratch[0] = '\0';
    if ( get_dirname( "./hi/there", scratch, sz ) || strcmp("./hi", scratch) )
    {
        ++errors;
        LOG("get_dirname fail with explicit relative path './hi/there' ['%s']\n", scratch);
    }

    scratch[0] = '\0';
    if ( get_dirname( "../hi/there", scratch, sz ) || strcmp("../hi", scratch) )
    {
        ++errors;
        LOG("get_dirname fail with explicit relative path '../hi/there' ['%s']\n", scratch);
    }

    scratch[0] = '\0';
    if ( get_dirname( "hi/there", scratch, sz ) || strcmp("hi", scratch) )
    {
        ++errors;
        LOG("get_dirname fail with relative path 'hi/there ['%s']'\n", scratch);
    }
    return errors;
}



int test_get_basename()
{
    static const size_t sz = 256;
    int errors = 0;
    char scratch[sz];

    #ifdef _UNIX_
    scratch[0] = '\0';
    if ( get_basename( "/hi/there", scratch, sz ) || strcmp("there", scratch) )
    {
        ++errors;
        LOG("get_basename fail with absolute path '/hi/there' ['%s']\n", scratch);
    }
    #else //windows
    scratch[0] = '\0';
    if ( get_basename( "c:/hi/there", scratch, sz ) || strcmp("there", scratch) )
    {
        ++errors;
        LOG("get_basename fail with absolute path 'c:/hi/there ['%s']'\n", scratch);
    }

    scratch[0] = '\0';
    if ( get_basename( "c:\\hi\\there", scratch, sz ) || strcmp("there", scratch) )
    {
        ++errors;
        LOG("get_basename fail with absolute path 'c:\\hi\\there ['%s']'\n", scratch);
    }
    #endif

    scratch[0] = '\0';
    if ( get_basename( "./hi/there", scratch, sz ) || strcmp("there", scratch) )
    {
        ++errors;
        LOG("get_basename fail with explicit relative path './hi/there' ['%s']\n", scratch);
    }

    scratch[0] = '\0';
    if ( get_basename( "../hi/there", scratch, sz ) || strcmp("there", scratch) )
    {
        ++errors;
        LOG("get_basename fail with explicit relative path '../hi/there' ['%s']\n", scratch);
    }

    scratch[0] = '\0';
    if ( get_basename( "hi/there", scratch, sz ) || strcmp("there", scratch) )
    {
        ++errors;
        LOG("get_basename fail with relative path 'hi/there ['%s']'\n", scratch);
    }
    return errors;
}



int test_get_absolute__add_include_dir()
{
    static const size_t sz = 256;
    int errors = 0;
    char scratch[sz];

    strcpy( scratch, "path_utils.c" );
    if ( get_absolute( scratch, sz ) != -1 )
    {
        LOG("get_absolute: file should not have been found ['%s']'\n", scratch);
        ++errors;
    }

    add_include_dir(".");
    if ( get_absolute( scratch, sz ) != 0 )
    {
        LOG("get_absolute: file not found ['%s']' in './'\n", scratch);
        ++errors;
    }

    strcpy( scratch, "non-existent-file.c" );
    if ( get_absolute( scratch, sz ) != -1 )
    {
        LOG("get_absolute: file should not have been found ['%s']' in './'\n", scratch);
        ++errors;
    }

    if ( get_absolute( scratch, 5 ) != 1 )
    {
        LOG("get_absolute: buffer should have been too small for '%s'\n", scratch);
        ++errors;
    }

    return errors;
}



int main()
{
    if ( test_is_definite() == 0 )
        LOG("is_definite passed!\n");

    if ( test_get_dirname() == 0 )
        LOG("get_dirname passed!\n");

    if ( test_get_basename() == 0 )
        LOG("get_basename passed!\n");

    if ( test_get_absolute__add_include_dir() == 0 )
        LOG("get_absolute__add_include_dir passed!\n");
    return 0;
}
