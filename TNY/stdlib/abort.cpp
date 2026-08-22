//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <stdlib.h>

#include "MTL/MTL.h"

void abort()
{
   MTL_halt(2);
}
