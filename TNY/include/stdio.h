//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include "UCL/stdio.h"

#define  FILENAME_MAX 256

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   int           fd;
   unsigned long pos;

} FILE;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

enum { SEEK_SET, SEEK_CUR, SEEK_END };


extern FILE*   fopen(const char* filename, const char* mode);
extern size_t  fseek(FILE* stream, long offset, int whence);
extern long    ftell(FILE* stream);
extern size_t  fread(void*, size_t, size_t, FILE* stream);
extern size_t  fwrite(const void*, size_t, size_t, FILE* stream);
extern int     fclose(FILE* stream);
extern int     feof(FILE* stream);
extern int     fflush(FILE* stream);

extern char*   fgets(char*, int, FILE*);
extern int     fgetc(FILE*);
extern int     fputc(int, FILE*);
extern int     fputs(const char*, FILE*);
extern int     fprintf(FILE*, const char*, ...);
extern int     vfprintf(FILE*, const char*, va_list);

extern int     puts(const char* s);
extern int     putchar(int c);
extern int     getchar();
extern char*   gets(char*);
extern int     printf(const char* format, ...);
extern int     vprintf(const char*, va_list);


#ifdef __cplusplus
}
#endif
