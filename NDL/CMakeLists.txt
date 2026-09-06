#--------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#--------------------------------------------------------------------------------

add_library(NDL STATIC
            Device.cpp
            Model.cpp
            Port.cpp
            util.cpp)

target_include_directories(NDL PUBLIC ..)

add_subdirectory(test)
