//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

namespace TST {

extern bool pass;

void fail(const char* file_, unsigned line_);

void print(const char* s_);

template <typename TYPE_L, typename TYPE_R>
inline bool expect_eq(TYPE_L      left_,
                      TYPE_R      right_,
                      const char* right_s_,
                      const char* file_,
                      unsigned    line_)
{
   if (TYPE_R(left_) == right_) 
      return true;

   fail(file_, line_);

   return false;
}

template <typename TYPE_L, typename TYPE_R>
inline bool expect_ne(TYPE_L      left_,
                      TYPE_R      right_,
                      const char* right_s_,
                      const char* file_,
                      unsigned    line_)
{
   if (TYPE_R(left_) != right_)
      return true;

   fail(file_, line_);

   return false;
}

} // namespace TST

#define EXPECT_EQ(left, right) \
   TST::pass &= TST::expect_eq(left, right, #right, __FILE__, __LINE__)

#define EXPECT_NE(left, right) \
   TST::pass &= TST::expect_ne(left, right, #right, __FILE__, __LINE__)

#define EXPECT_TRUE(expr) \
   TST::pass &= TST::expect_eq(true, bool(expr), #expr, __FILE__, __LINE__)

#define EXPECT_FALSE(expr) \
   TST::pass &= TST::expect_eq(false, bool(expr), #expr, __FILE__, __LINE__)
