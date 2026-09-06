#--------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#--------------------------------------------------------------------------------

add_executable(testNDL
               testMain.cpp
               testNDL.cpp)

target_link_libraries(testNDL NDL STB)

add_test(NAME testNDL COMMAND testNDL)
