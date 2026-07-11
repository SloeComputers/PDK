//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdarg>
#include <cstdio>
#include <mutex>

inline FILE* LOGFP()
{
   static FILE* log_fp = []()
   {
      FILE* fp = fopen("/tmp/PDK_vst3.log", "a");
      if (fp == nullptr)
         fp = stdout;

      fprintf(fp,
         "--------------------------------------------------------------------------------\n");

      return fp;
   }();

   return log_fp;
}

inline void LOG(const char* format, ...)
{
   static std::mutex mutex{};

   std::lock_guard<std::mutex> lock(mutex);

   FILE* fp = LOGFP();

   va_list ap;
   va_start(ap, format);
   vfprintf(fp, format, ap);
   va_end(ap);

   fflush(fp);
}
