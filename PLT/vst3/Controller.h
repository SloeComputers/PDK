//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/main/pluginfactory.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

#include "Config.h"

class Controller : public Steinberg::Vst::EditController
{
public:
   Controller() = default;

   static const Steinberg::FUID& uid()
   {
      const static Steinberg::FUID UID{CONTROLLER_UID};
      return UID;
   }

   static void registerClass(Steinberg::CPluginFactory* factory_)
   {
      const static Steinberg::PClassInfo2 class_info(uid().toTUID(),
                                                     Steinberg::PClassInfo::kManyInstances,
                                                     kVstComponentControllerClass,
                                                     PDK_PROJ_NAME "_Controller",
                                                     0,
                                                     "",
                                                     nullptr,
                                                     PDK_PROJ_VERSION,
                                                     kVstVersionString);

      factory_->registerClass(&class_info, construct);
   }

   static constexpr Steinberg::Vst::ParamID PARAM_ID_PROGRAM_CHANGE = 1000;

private:
   static Steinberg::FUnknown* construct(void*)
   {
      return static_cast<Steinberg::Vst::IEditController*>(new Controller());
   }

   Steinberg::tresult initialize(FUnknown* context_) override
   {
      Steinberg::tresult result = Steinberg::Vst::EditController::initialize(context_);
      if (result != Steinberg::kResultOk)
         return result;

      parameters.addParameter(STR16("Program"),
                              nullptr,
                              127,
                              0,
                              Steinberg::Vst::ParameterInfo::kCanAutomate |
                              Steinberg::Vst::ParameterInfo::kIsProgramChange,
                              PARAM_ID_PROGRAM_CHANGE);

      return Steinberg::kResultOk;
   }
};
