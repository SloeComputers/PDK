//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SIG/ReSample.h"
#include "SIG/osc/Sine.h"

#include "STB/Test.h"

#include <cstdio>

using namespace SIG;

TEST(SIG, resample)
{
   constexpr unsigned SOURCE_RATE = 49096;
   constexpr unsigned OUTPUT_RATE = 48000;

   SIG::osc::Sine osc{};

   osc.sync();
   osc.setFreq(440.0, SOURCE_RATE);

   SIG::ReSample<SIG::osc::Sine, /* N */ 2, 2> re_sample{osc, SOURCE_RATE};

   re_sample.setOutRate(OUTPUT_RATE);

   unsigned    pos_cross = 0;
   SIG::Signal last_out  = -0.1;

   for(unsigned i = 0; i < OUTPUT_RATE; ++i)
   {
      SIG::Signal out = re_sample();

      if ((out >= 0) && (last_out < 0))
         pos_cross++;

      last_out = out;
   }

   EXPECT_GE(441, pos_cross);
   EXPECT_LE(439, pos_cross);
}
