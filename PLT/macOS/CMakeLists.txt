#-------------------------------------------------------------------------------
# Copyright (c) 2022 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# cmake configuration for MacOS builds

find_package(SDL2 REQUIRED)

add_library(PLT STATIC
    MIDI.cpp
    ../SDL2/Audio.cpp
    ../SDL2/Event.cpp
    ../SDL2/Frame.cpp
    ../SDL2/Image.cpp
    ../SDL2/Bitmap.cpp
    ../Stub/Info.cpp
    ../Stub/Sounder.cpp
    ../POSIX/Yield.cpp
    ../POSIX/File.cpp
    ../POSIX/Rtc.cpp
    ../POSIX/Socket.cpp)

target_compile_definitions(PLT
    PUBLIC PDK_FRAME_BUFFERED)

target_include_directories(PLT
    PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(PLT
    PUBLIC MIDI
    PRIVATE ${SDL2_LIBRARIES}
    INTERFACE "-framework CoreMIDI" "-framework CoreFoundation")
