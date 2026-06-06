//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

class IPluginFactory;

extern IPluginFactory* pluginFactoryForVST3();

extern "C" __attribute__((visibility("default"))) IPluginFactory* GetPluginFactory()
{
   return pluginFactoryForVST3();
}
