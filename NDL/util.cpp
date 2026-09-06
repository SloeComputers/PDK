//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdarg>
#include <cstdio>
#include <string>

#include "NDL.h"

bool NDL::error(const char* format, ...)
{
   va_list ap;

   fprintf(stderr, "NDL::ERROR - ");

   va_start(ap, format);
   vfprintf(stderr, format, ap);
   va_end(ap);

   fprintf(stderr, "\n");

   return false;
}

bool NDL::warn(const char* format, ...)
{
   va_list ap;

   fprintf(stderr, "NDL::WARNING - ");

   va_start(ap, format);
   vfprintf(stderr, format, ap);
   va_end(ap);

   fprintf(stderr, "\n");

   return false;
}

FILE* NDL::openForWrite(const Model* model_, const char* dir_, const char* ext_)
{
   std::string filename{};

   filename = dir_;
   filename += '/';
   filename += model_->getName();
   filename += '.';
   filename += ext_;

   FILE* fp = fopen(filename.c_str(), "w");
   if (fp == nullptr)
   {
      error("Failed to open '%s'", filename.c_str());
   }

   printf("Write '%s'\n", filename.c_str());

   return fp;
}

NDL::Model* NDL::readXML(const char* filename_)
{
   FILE* fp = fopen(filename_, "r");
   if (fp == nullptr)
   {
      error("Failed to open '%s'", filename_);
      return nullptr;
   }

   fclose(fp);

   NDL::Model* model = new Model("");

   return model;
}
