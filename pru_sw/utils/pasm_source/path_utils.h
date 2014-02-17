/*
 * path_utils.h
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
// File     : path_utils.h
//
// Description:
//     File path utilities to make things eaiser...
//
============================================================================*/

#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include <stdlib.h>

/** Determines whether the filename is either absolute or explicitly relative to
 * the current directory (as indicated with a leading . or .. */
int is_definite( const char * filename );

/** Add the given directory to the list of directories that are searched for
 * #include dirctives. */
int add_include_dir( const char * dirname );

/** Search through the include paths for the given filename.
 * This function will modify the filename variable to return the absolute
 * filename if found. 
 * @return 0 if filename was found, -1 otherwise.
 * @return -1 if filename was not found
 * @return 1 if the filename _was_ found but sz is too small.
 */
int get_absolute( char * filename, const size_t sz );

/** Determines directory component of filename path.
 * @return -1 if sz is too small, 0 otherwise
 */
int get_dirname( const char * filename, char * dir, const size_t sz);

/** Determines base name of file component of filename path.
 * @return -1 if sz is too small, 0 otherwise
 */
int get_basename( const char * filename, char * base, const size_t sz);

#endif // PATH_UTILS_H
