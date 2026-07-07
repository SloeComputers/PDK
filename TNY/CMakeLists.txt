#-------------------------------------------------------------------------------
# Copyright (c) 2022 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(TNY STATIC
            stdio/fprintf.cpp
            stdio/feof.cpp
            stdio/fflush.cpp
            stdio/fgetc.cpp
            stdio/fputc.cpp
            stdio/fstreams.cpp
            stdio/getchar.cpp
            stdio/printf.cpp
            stdio/putchar.cpp
            stdio/puts.cpp
            stdio/scanf.cpp
            stdlib/abort.cpp
            stdlib/exit.cpp
            stdlib/system.cpp
            stdlib/malloc.cpp
            time/clock.cpp
            time/time.cpp
            unistd/sleep.cpp
            unistd/usleep.cpp)

target_include_directories(TNY PUBLIC include)

target_link_libraries(TNY PUBLIC MTL)
