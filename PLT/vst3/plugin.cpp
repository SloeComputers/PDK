//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "public.sdk/source/main/pluginfactory.h"

#include "Processor.h"
#include "Controller.h"

using namespace Steinberg;

extern int plugin_main();

IPluginFactory* pluginFactoryForVST3()
{
   if (gPluginFactory == nullptr)
   {
      static PFactoryInfo factory_info(PDK_PROJ_OWNER,
                                       PDK_PROJ_URL,
                                       PDK_PROJ_EMAIL,
                                       PFactoryInfo::kNoFlags);

      gPluginFactory = new CPluginFactory(factory_info);

      Processor::registerClass(gPluginFactory);
      Controller::registerClass(gPluginFactory);
   }
   else
   {
      gPluginFactory->addRef();
   }

   return gPluginFactory;
}
