//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Types.h"

namespace SIG {

template <typename SOURCE, unsigned N, unsigned LOG2_M = 8>
class ReSample
{
public:
   ReSample(SOURCE& source_, unsigned sample_rate_in_)
      : source(source_)
      , sample_rate_in(sample_rate_in_)
   {
      // TODO replace this point sample with sinc
      for(unsigned i = 0; i < FILTER_SIZE; ++i)
      {
         signed j = i - (FILTER_SIZE / 2);

         if ((j >= -M/2) && (j < +M/2))
            filter[i] = 1.0;
         else
            filter[i] = 0.0;
      }
   }

   void setOutRate(unsigned sample_rate_out_)
   {
      delta_t = TIME_ONE * SIG::Float(sample_rate_in) / sample_rate_out_;
   }

   Signal operator()()
   {
      uint32_t offset = t >> (LOG2_T - LOG2_M);
      unsigned j      = input_first;
      Signal   signal = 0;

      for(unsigned i = 0; i < INPUT_SIZE; ++i)
      {
         signal += filter[i * M + offset] * input[j];

         if (++j == INPUT_SIZE)
            j = 0;
      }

      t += delta_t;
      while(t >= TIME_ONE)
      {
         t -= TIME_ONE;
         input[input_first] = source();

         if (++input_first == INPUT_SIZE)
            input_first = 0;
      }

      return signal;
   }

private:
   static const unsigned LOG2_T      = 24;
   static const signed   M           = 1 << LOG2_M;
   static const unsigned INPUT_SIZE  = N * 2 + 1;
   static const unsigned FILTER_SIZE = INPUT_SIZE * M;
   static const uint32_t TIME_ONE    = 1 << LOG2_T;

   SOURCE&     source;
   unsigned    sample_rate_in;
   SIG::Signal input[2 * N + 1] = {};
   SIG::Signal filter[FILTER_SIZE];
   unsigned    input_first{0};
   int32_t     t{0};
   uint32_t    delta_t{TIME_ONE};
};

} // namespace SIG
