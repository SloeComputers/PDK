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
      static Steinberg::PClassInfo2 class_info(uid().toTUID(),
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

private:
   static Steinberg::FUnknown* construct(void*)
   {
      return static_cast<Steinberg::Vst::IEditController*>(new Controller());
   }
};
