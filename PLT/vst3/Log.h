//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdio>

inline FILE* LOGFP()
{
   static FILE* fp = nullptr;

   if (fp == nullptr)
   {
      fp = fopen("/tmp/PDK_vst3.log", "a");
      if (fp == nullptr)
         fp = stdout;

      fprintf(fp,
         "--------------------------------------------------------------------------------\n");
   }

   return fp;
}

inline void LOG(const char* format, ...)
{
   FILE* fp = LOGFP();

   va_list ap;
   va_start(ap, format);
   vfprintf(fp, format, ap);
   va_end(ap);

   fflush(fp);
}
