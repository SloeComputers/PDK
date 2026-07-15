#-------------------------------------------------------------------------------
# Copyright (c) 2022 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

function(arcxx name)

   add_custom_command(
      COMMENT
         "Create C++ archive ${name}"
      COMMAND
         ${CMAKE_SOURCE_DIR}/PDK/scripts/arcxx.py
             -o ${CMAKE_CURRENT_BINARY_DIR}/arcxx_${name}.cpp
             -r media/${name} ${CMAKE_SOURCE_DIR}/media/${name}
      DEPENDS
         ${CMAKE_SOURCE_DIR}/media/${name}
      OUTPUT
         ${CMAKE_CURRENT_BINARY_DIR}/arcxx_${name}.cpp
      )

endfunction()

#-------------------------------------------------------------------------------

function(autogen_py name)

   set(ARG "")
   if(ARGN)
      list(GET ARGN 0 ARG)
   endif()

   add_custom_command(
      COMMENT
         "Auto-generate ${name}"
      COMMAND
         ${CMAKE_COMMAND}
            -E env PYTHONPATH=${CMAKE_SOURCE_DIR}/PDK/python
            python3 ${CMAKE_CURRENT_SOURCE_DIR}/${name}.py ${ARG}
      DEPENDS
         ${CMAKE_CURRENT_SOURCE_DIR}/${name}.py
         ${INPUT_FILE}
      OUTPUT
         ${CMAKE_CURRENT_BINARY_DIR}/${name}.h
         ${CMAKE_CURRENT_BINARY_DIR}/${name}.cpp
      )

endfunction()

#-------------------------------------------------------------------------------
# Entry point

get_directory_property(has_parent PARENT_DIRECTORY)

if(has_parent)
   set(CMAKE_CXX_STANDARD          20   PARENT_SCOPE)
   set(CMAKE_CXX_STANDARD_REQUIRED True PARENT_SCOPE)
else()
   cmake_minimum_required(VERSION 3.20)

   project(PDK
           LANGUAGES CXX C ASM
           VERSION 0.6)
endif()

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT DEFINED BUILD_TESTING)
    set(BUILD_TESTING OFF CACHE BOOL "Build unit tests")
endif()
include(CTest)

#-------------------------------------------------------------------------------
# Introspect to determine PDK project variables...
#    PDK_TARGET
#    PDK_MACHINE
#    PDK_PROJ_COMMIT

# Determine current git status
execute_process(COMMAND git log --pretty=format:%H -n 1
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE PDK_PROJ_COMMIT)

if(NOT PDK_PROJ_OWNER)
   execute_process(COMMAND git config get user.name
                   OUTPUT_VARIABLE PDK_PROJ_OWNER
                   OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

if(NOT PDK_PROJ_EMAIL)
   execute_process(COMMAND git config get user.email
                   OUTPUT_VARIABLE PDK_PROJ_EMAIL
                   OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

if(NOT PDK_TARGET)
   if(DEFINED ENV{PDK_TARGET})
      set(PDK_TARGET $ENV{PDK_TARGET})
   else()
      set(PDK_TARGET native)
   endif()
endif()

if("${PDK_TARGET}" STREQUAL "native")

   set(PDK_NATIVE  TRUE)
   set(PDK_TARGET  ${CMAKE_HOST_SYSTEM_NAME})
   set(PDK_MACHINE ${CMAKE_HOST_SYSTEM_PROCESSOR})

   if("${PDK_TARGET}" STREQUAL "Darwin")
      set(PDK_TARGET "macOS")
   endif()

   add_subdirectory(TGT)

elseif("${PDK_TARGET}" STREQUAL "vst3")

   set(PDK_NATIVE  TRUE)
   set(PDK_MACHINE ${CMAKE_HOST_SYSTEM_PROCESSOR})

   add_subdirectory(TGT)

else()

   set(PDK_NATIVE FALSE)
   set(CMAKE_ASM_FLAGS_DEBUG   "")
   set(CMAKE_ASM_FLAGS_RELEASE "")

   add_subdirectory(TGT)
   add_subdirectory(UCL)
   add_subdirectory(MTL)
   add_subdirectory(TNY)

endif()

#-------------------------------------------------------------------------------

message("--------------------------------------------------------------------------------")
message("CMAKE_PROJECT_NAME    = ${CMAKE_PROJECT_NAME}")
message("CMAKE_PROJECT_VERSION = ${CMAKE_PROJECT_VERSION}")
message("CMAKE_BUILD_TYPE      = ${CMAKE_BUILD_TYPE}")
message("--------------------------------------------------------------------------------")
message("PDK_PROJ_COMMIT    = ${PDK_PROJ_COMMIT}")
message("PDK_PROJ_OWNER     = ${PDK_PROJ_OWNER}")
message("PDK_PROJ_EMAIL     = ${PDK_PROJ_EMAIL}")
message("PDK_TARGET         = ${PDK_TARGET}")
message("PDK_NATIVE         = ${PDK_NATIVE}")
message("PDK_MACHINE        = ${PDK_MACHINE}")
message("--------------------------------------------------------------------------------")

if(has_parent)
    set(PDK_NATIVE ${PDK_NATIVE} PARENT_SCOPE)
endif()

#-------------------------------------------------------------------------------

if(NOT PDK_TARGET STREQUAL "ATtiny84" AND NOT PDK_TARGET STREQUAL "ATtiny85")

add_subdirectory(STB)
add_subdirectory(USB)
add_subdirectory(SIG)
add_subdirectory(MIDI)
add_subdirectory(PLT)
add_subdirectory(GUI)
add_subdirectory(TRM)

endif()

add_subdirectory(HWR)

