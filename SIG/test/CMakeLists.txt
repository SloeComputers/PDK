#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PDK_NATIVE})

   foreach(variant IN LISTS SIG_VARIANTS)

       set(lib  SIG_${variant})
       set(test testSIG_${variant})

       add_executable(${test}
                      testMain.cpp
                      testConv.cpp
                      testDelay.cpp
                      testDelayN.cpp
                      testDelayV.cpp
                      testExpSlew.cpp
                      testGain.cpp
                      testLinSlew.cpp
                      testLogPot.cpp
                      testReSample.cpp
                      test_envAdsr.cpp
                      test_clipHard.cpp
                      test_clipNo.cpp
                      test_clipPoly.cpp
                      test_clipPoly5.cpp
                      test_clipTanh.cpp
                      test_oscPulse.cpp
                      test_oscSquare.cpp)

       target_link_libraries(${test} ${lib} STB)

       add_test(NAME ${test} COMMAND ${test} )

   endforeach()

endif()
