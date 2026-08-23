#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(PLT STATIC
            Audio.cpp
            Event.cpp
            Frame.cpp
            Image.cpp
            NullBitmap.cpp
            MIDI.cpp
            Sounder.cpp
            Yield.cpp
            Info.cpp
            File.cpp)

target_link_libraries(PLT PUBLIC TNY MTL)
