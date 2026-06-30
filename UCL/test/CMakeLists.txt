#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PDK_NATIVE})

   add_executable(testUCL
                  test_main.cpp
                  test_ctype.cpp
                  test_sprintf.cpp)

   target_link_libraries(testUCL PRIVATE UCL)

   add_test(NAME testUCL COMMAND testUCL)

endif()
