//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdio.h"
#include "string.h"

#include "test.h"

void test_sprintf()
{
   char buffer[256];

   snprintf(buffer, sizeof(buffer), "hello");
   EXPECT_EQ(0, strcmp(buffer, "hello"));

   snprintf(buffer, sizeof(buffer), "hello%%");
   EXPECT_EQ(0, strcmp(buffer, "hello%"));

   snprintf(buffer, sizeof(buffer), "hello%u", 1);
   EXPECT_EQ(0, strcmp(buffer, "hello1"));
   TST::print(buffer);

   snprintf(buffer, sizeof(buffer), "hello%u,%i", 1, 2);
   EXPECT_EQ(0, strcmp(buffer, "hello1,2"));
   TST::print(buffer);

   snprintf(buffer, sizeof(buffer), "hello%u,%i,%d", 1, 2, 3);
   EXPECT_EQ(0, strcmp(buffer, "hello1,2,3"));
   TST::print(buffer);

   snprintf(buffer, sizeof(buffer), "hello%c_", ';');
   EXPECT_EQ(0, strcmp(buffer, "hello;_"));
   TST::print(buffer);

   snprintf(buffer, sizeof(buffer), "hello, %s", "world");
   EXPECT_EQ(0, strcmp(buffer, "hello, world"));
   TST::print(buffer);

   char* s = buffer;
   char* end_s = s + sizeof(buffer);

   s += snprintf(s, end_s - s, "Program  : %s (%s)\n", "test", "native");
   s += snprintf(s, end_s - s, "Author   : Copyright (c) 2025 John D. Haughton\n");
   s += snprintf(s, end_s - s, "License  : MIT\n");
   s += snprintf(s, end_s - s, "Version  : %s\n", "0.1");
   s += snprintf(s, end_s - s, "Commit   : %s\n", "abcdef");
   s += snprintf(s, end_s - s, "Built    : %s %s\n", __TIME__, __DATE__);
#if defined(__clang__)
   s += snprintf(s, end_s - s, "Compiler : Clang %s\n", __VERSION__);
#elif defined(__GNUC__)
   s += snprintf(s, end_s - s, "Compiler : GCC %s\n", __VERSION__);
#else
   s += snprintf(s, end_s - s, "Compiler : %s\n", __VERSION__);
#endif
   s += snprintf(s, end_s - s, "Target   : %s %s\n", "native", "arm64");
   TST::print(buffer);
}
