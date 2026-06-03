//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cstdlib>

#include "Base.h"

namespace SIG::osc {

template <unsigned LOG2_SIZE = 10>
class WaveTable : public Base
{
public:
   WaveTable() = default;

   Signal operator()()
   {
      unsigned index = phase >> SHIFT;

      phase += delta;

      return gain(table[index]);
   }

   Signal operator()(Signal mod_)
   {
      unsigned index = phase >> SHIFT;

      phase += modDelta(mod_);

      return gain(table[index]);
   }

   void computeWave()
   {
      Signal local[SIZE];

      for(unsigned i = 0; i < SIZE; ++i)
      {
         local[i] = wavetableSample(Float(i) / SIZE);
      }

      updateWave(local);
   }

   const Signal* getTable(size_t& size) const
   {
      size = SIZE;
      return table;
   }

protected:
   virtual Signal wavetableSample(Float t) = 0;

private:
   //! Rewrite wave tabel with new data
   void updateWave(Signal* data)
   {
      Signal min = +1.0;
      Signal max = -1.0;

      for(unsigned i = 0; i < SIZE; ++i)
      {
         Signal value = data[i];

         if(value > max) max = value;
         if(value < min) min = value;
      }

      // Rescale to -1..+1
      for(unsigned i = 0; i < SIZE; ++i)
      {
         data[i] = (2.0 * (data[i] - min) / (max - min)) - 1.0;
      }

      // Final copy to live wave table
      for(unsigned i = 0; i < SIZE; ++i)
      {
         table[i] = data[i];
      }
   }

   static constexpr unsigned SIZE      = 1 << LOG2_SIZE;
   static constexpr unsigned SHIFT     = sizeof(UPhase) * 8 - LOG2_SIZE;

   Signal table[SIZE];
};

} // namespace SIG::osc
