//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>

#include "test.h"

extern void test_ctype();
extern void test_sprintf();

bool TST::pass{true};

void TST::fail(const char* file_, unsigned line_no_)
{
   printf("FAIL: %s:%u\n", file_, line_no_);
}

void TST::print(const char* s_)
{
   printf("\"%s\"\n", s_);
}

int main()
{
   test_ctype();
   test_sprintf();

   if (TST::pass)
      printf("PASSED\n");
   else
      printf("FAILED\n");

   return TST::pass ? 0 : 1;
}
