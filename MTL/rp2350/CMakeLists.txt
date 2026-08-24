#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(MTL STATIC
            MTL_init.cpp
            ../MTL_halt.cpp
            ../MTL_fault.cpp
            ../rp2xxx/Uart.cpp
            ../rp2xxx/MTL_alert.cpp
            ../rp2xxx/MTL_console.cpp
            ../core/CortexM33/MTL_excep.cpp
            MTL_start_core.cpp
            MTL_clock.cpp
            MTL_mutex.cpp
            Dma.cpp
            Config.cpp)

target_include_directories(MTL PUBLIC include)
