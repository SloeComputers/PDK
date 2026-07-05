//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------


#pragma once

#include <cmath>

#include "Types.h"

namespace SIG {

//! Windowed-sinc polyphase FIR resampler with linear phase interpolation
template <typename SOURCE, unsigned N, unsigned LOG2_PHASE_STEPS = 8>
class ReSample
{
public:
   ReSample(SOURCE& source_, unsigned sample_rate_in_)
      : source(source_)
      , sample_rate_in(sample_rate_in_)
   {
      for(unsigned phase_idx = 0; phase_idx <= PHASE_STEPS; ++phase_idx)
      {
         Float phase = Float(phase_idx) / PHASE_STEPS;

         Float sum_coef = 0.0;
         for(unsigned i = 0; i < INPUT_SIZE; ++i)
         {
            Float x    = phase - (i - N);
            Float coef = sinc(x) * window(x);
            table[phase_idx][i] = coef;
            sum_coef += coef;
         }

         // Normalize as windowed sinc() will not convolve to 1.0
         for(unsigned i = 0; i < INPUT_SIZE; ++i)
         {
            table[phase_idx][i] /= sum_coef;
         }
      }
   }

   //! Set the output sampling frequency (Hz)
   void setOutRate(unsigned sample_rate_out_)
   {
      delta_phase = Float(sample_rate_in) / sample_rate_out_;
   }

   //! Get next sample
   Signal operator()()
   {
      Signal   signal = 0;
      unsigned j      = input_first;
      unsigned k      = unsigned(phase * PHASE_STEPS);
      Float    a      = phase * PHASE_STEPS - k;

      for(unsigned i = 0; i < INPUT_SIZE; ++i)
      {
         Float coef1 = table[k    ][i];
         Float coef2 = table[k + 1][i];
         Float coef  = coef1 + a * (coef2 - coef1);

         signal += coef * input[j];

         if (++j == INPUT_SIZE)
            j = 0;
      }

      phase += delta_phase;
      while(phase >= 1.0)
      {
         phase -= 1.0;
         input[input_first] = source();

         if (++input_first == INPUT_SIZE)
            input_first = 0;
      }

      return signal;
   }

private:
   //! Classic sinc()
   static double sinc(double x)
   {
      if (fabs(x) < 1e-12)
         return 1.0;

      double t = M_PI * x;
      return sin(t) / t;
   }

   //! A sinc() based windowing function
   static double window(double x)
   {
      return sinc(x / N);
   }

   static constexpr signed   PHASE_STEPS = 1 << LOG2_PHASE_STEPS;
   static constexpr unsigned INPUT_SIZE  = N * 2 + 1;

   SOURCE&  source;
   unsigned sample_rate_in;                      //!< Fin
   Float    delta_phase{};                       //!< Fin / Fout
   unsigned input_first{0};                      //!< Index into input circular buffer
   Signal   input[2 * N + 1] = {};               //!< Circular buffer for input samples
   Float    phase{0.0};                          //!< Current "phase" 0.0..1.0
   Float    table[PHASE_STEPS + 1][INPUT_SIZE];  //!< Pre-computed filter
};

} // namespace SIG
