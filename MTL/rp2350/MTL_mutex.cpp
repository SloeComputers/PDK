//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/Mutex.h"

#include "MTL/rp2350/Sio.h"

static unsigned mutex_id{1};
static MTL::Sio sio{};

MTL::Mutex::Mutex()
{
   // Use spin-lock 0 to protect allocation of mutex handles
   while(not sio.try_lock(0));

   handle = mutex_id++;

   sio.unlock(0);
}

bool MTL::Mutex::try_lock()
{
   return sio.try_lock(handle);
}

void MTL::Mutex::lock()
{
   while(not try_lock());
}

void MTL::Mutex::unlock()
{
   return sio.unlock(handle);
}
