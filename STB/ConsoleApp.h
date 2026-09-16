//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "STB/License.h"
#include "STB/Option.h"

namespace STB {

//! A generic class to simplify the creation of user friendly console applications
class ConsoleApp
{
public:
   ConsoleApp(const char* program_,
              const char* description_,
              const char* link_,
              const char* author_,
              const char* copyright_year_,
              const char* args_help_ = nullptr)
#if !defined(PDK_NCONSOLE)
      : name(program_)
      , program(program_)
      , description(description_)
      , link(link_)
      , author(author_)
      , copyright_year(copyright_year_)
#endif
   {}

   void licGPL2() { lic_gpl2 = true; }

   int parseArgsAndStart(int argc, const char* argv[])
   {
#if !defined(PDK_NCONSOLE)
#if defined(PDK_TARGET_Emscripten)
      // TODO this is just a confidence test
      static const char* local_argv[] = {"fred"};

      argc = 1;
      argv = local_argv;
#endif
      if (argc != 0)
      {
         name = extractFilename(argv[0]);
      }

      for(int i = 1; i < argc; ++i)
      {
         OptionBase* option = OptionBase::find(argv[i]);
         if(option != nullptr)
         {
            if (option->isGlob())
            {
               (void) option->set(argv[i]);
            }
            else
            {
               const char* next_arg = (i + 1) < argc ? argv[i + 1] : nullptr;

               if(option->set(next_arg))
               {
                  if(next_arg)
                  {
                     ++i;
                  }
                  else
                  {
                     error("option value for %s is missing", argv[i]);
                  }
               }
            }
         }
         else if(argv[i][0] == '-')
         {
            error("unknown option %s", argv[i]);
         }
      }

      if(opt_version) showVersion();
      if(opt_help)    showHelp();
#endif

      return startConsoleApp();
   }

protected:
#if !defined(PDK_NCONSOLE)
   const char* name;
   const char* program;
   const char* description;
   const char* link;
   const char* author;
   const char* copyright_year;
#endif

   void error(const char* format, ...)
   {
#if !defined(PDK_NCONSOLE)
      va_list ap;

      fprintf(stderr, "%s: ERROR - ", name);

      va_start(ap, format);
      vfprintf(stderr, format, ap);
      va_end(ap);

      fprintf(stderr, "\n");
#endif

      exit(1);
   }

   virtual void showExtraHelp() {}

   virtual int startConsoleApp() = 0;

private:
#if !defined(PDK_NCONSOLE)
   Option<bool> opt_version{'v', "version", "Display version information"};
   Option<bool> opt_help{   'h', "help",    "Display this help"};

   static const char* extractFilename(const char* path)
   {
      // Windows is not supported
      const char* filename = strrchr(path, '/');

      return filename != nullptr ? filename + 1
                                 : path;
   }

   static void normal() { printf("\033[0m"); }
   static void bold()   { printf("\033[1m"); }

   void showVersion()
   {
      printf("\n");
      printf("Program      : %s\n", program);
      printf("Description  : %s\n", description);
      printf("Author       : %s\n", author);
      printf("Version      : %s\n", PDK_PROJ_VERSION);
      if (link != nullptr)
      {
         printf("Link         : %s\n", link);
      }
      printf("Commit       : %s\n", PDK_PROJ_COMMIT);
      printf("Built        : %s %s\n", __TIME__, __DATE__);
      printf("Compiler     : %s\n", __VERSION__);
#if defined(__arm__)
      printf("Architecture : ARM-32\n");
#elif defined(__aarch64__)
      printf("Architecture : ARM-64\n");
#elif defined(__mips__)
      printf("Architecture : MIPS\n");
#elif defined(__powerpc__)
      printf("Architecture : PowerPC\n");
#elif defined(__sparc__)
      printf("Architecture : SPARC\n");
#elif defined(__i386__)
      printf("Architecture : x86-32\n");
#elif defined(__amd64__)
      printf("Architecture : AMD-64 (x86-64)\n");
#endif
      printf("\n");
      bold();
      printf("Copyright (c) %s %s\n", copyright_year, author);
      normal();
      printf("\n");
      if (lic_gpl2) printf("%s", GPL2_LICENSE);
      else          printf("%s", MIT_LICENSE);
      printf("\n");
      printf("\n");

      exit(0);
   }

   void showHelp()
   {
      printf("\n");
      bold();
      printf("NAME\n");
      normal();
      printf("     %s - %s\n", program, description);
      printf("\n");
      bold();
      printf("SYNOPSIS\n");
      normal();
      printf("     %s [options] %s\n", name, OptionBase::getSynopsisSuffix());
      printf("\n");
      bold();
      printf("OPTIONS\n");
      normal();

      OptionBase::printHelpAll();

      printf("\n");

      showExtraHelp();

      exit(0);
   }
#endif

   bool lic_gpl2{false};
};

} // namespace STB

