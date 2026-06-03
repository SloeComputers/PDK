//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cmath>
#include <cstdlib>

#include "WaveTable.h"

namespace SIG::osc {

template <unsigned N>
class Additive : public WaveTable<>
{
public:
   Additive() = default;

   Float a[N + 1] = {};
   Float b[N + 1] = {};

private:
   Signal wavetableSample(Float t) override
   {
      Signal value = a[0] / 2.0;

      for(size_t n = 1; n <= N; n++)
      {
#if defined(SIG_FL32)
         value += a[n] * sinf(n * t * float(2 * M_PI));
         value += b[n] * cosf(n * t * float(2 * M_PI));
#elif defined(SIG_FL64)
         value += a[n] * sin(n * t * 2 * M_PI);
         value += b[n] * cos(n * t * 2 * M_PI);
#endif
      }

      return value;
   }
};

} // namespace SIG::osc
