#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

set(PDK_TARGET vst3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-volatile")

if(NOT DEFINED ENV{VST_SDK})
    message(FATAL_ERROR "VST_SDK environment variable is not set.")
endif()

set(VST3SDK_SOURCE_DIR "$ENV{VST_SDK}" CACHE PATH "VST3 SDK path")

if(NOT EXISTS "${VST3SDK_SOURCE_DIR}")
    message(FATAL_ERROR
        "VST_SDK points to a non-existent directory:\n"
        "  ${VST3SDK_SOURCE_DIR}"
    )
endif()

set(SMTG_ENABLE_VST3_HOSTING_EXAMPLES 0)
set(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES  0)
set(SMTG_ENABLE_VSTGUI_SUPPORT        0)
set(SMTG_CREATE_PLUGIN_LINK           0)

set(SMTG_VSTGUI_ROOT "${VST3SDK_SOURCE_DIR}")
