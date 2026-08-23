#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MIDI STATIC
            Controller/Akai_MIDImix.cpp
            Controller/Akai_MPKmini.cpp
            Controller/Novation_LaunchKey.cpp
            $<$<BOOL:${PDK_NATIVE}>:Decoder.cpp>)

target_include_directories(MIDI PUBLIC ..)

target_link_libraries(MIDI PUBLIC STB)

if(BUILD_TESTING)
   add_subdirectory(test)
endif()
