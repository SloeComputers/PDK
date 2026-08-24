#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# cmake configuration for VST3 builds

add_subdirectory(${VST3SDK_SOURCE_DIR}/vst3sdk ${PROJECT_BINARY_DIR}/vst3sdk)

add_library(PLT STATIC
    plugin.cpp
    Processor.cpp
    ../Stub/MIDI.cpp)

target_link_libraries(PLT
    PUBLIC MIDI SIG_FL64 sdk TGT TGT_PLG)
