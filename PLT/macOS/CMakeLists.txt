#-------------------------------------------------------------------------------
# Copyright (c) 2022 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# cmake configuration for MacOS builds

find_package(SDL3 REQUIRED)

add_library(PLT STATIC
    MIDI.cpp
    ../SDL3/Audio.cpp
    ../SDL3/Event.cpp
    ../SDL3/Frame.cpp
    ../SDL3/Image.cpp
    ../SDL3/Bitmap.cpp
    ../Stub/Info.cpp
    ../Stub/Sounder.cpp
    ../POSIX/Yield.cpp
    ../POSIX/File.cpp
    ../POSIX/Rtc.cpp
    ../POSIX/Socket.cpp)

target_compile_definitions(PLT
    PUBLIC PDK_FRAME_BUFFERED)

target_include_directories(PLT
    PRIVATE ${SDL3_INCLUDE_DIRS})

target_link_libraries(PLT
    PUBLIC MIDI
    PRIVATE ${SDL3_LIBRARIES}
    INTERFACE "-framework CoreMIDI" "-framework CoreFoundation")
