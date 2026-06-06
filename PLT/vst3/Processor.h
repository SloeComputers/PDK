//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "public.sdk/source/main/pluginfactory.h"
#include "public.sdk/source/vst/vstaudioeffect.h"

#include "Config.h"

class Processor : public Steinberg::Vst::AudioEffect
{
public:
   Processor();

   static const Steinberg::FUID& uid()
   {
      const static Steinberg::FUID UID{PROCESSOR_UID};
      return UID;
   }

   static void registerClass(Steinberg::CPluginFactory* factory_)
   {
      static Steinberg::PClassInfo2 class_info(uid().toTUID(),
                                               Steinberg::PClassInfo::kManyInstances,
                                               kVstAudioEffectClass,
                                               PDK_PROJ_NAME,
                                               Steinberg::Vst::kDistributable,
                                               Steinberg::Vst::PlugType::kInstrumentSynth,
                                               nullptr,
                                               PDK_PROJ_VERSION,
                                               kVstVersionString);

      factory_->registerClass(&class_info, construct);
   }

private:
   Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) override;
   Steinberg::tresult PLUGIN_API canProcessSampleSize(int32_t sample_size) override;
   Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) override;
   Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data) override;

   static Steinberg::FUnknown* construct(void*)
   {
      return static_cast<Steinberg::Vst::IAudioProcessor*>(new Processor());
   }

   template <typename SAMPLE>
   void render(Steinberg::Vst::ProcessData& data, int32_t start, int32_t end, bool& silent);
};
