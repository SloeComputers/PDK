//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief

#pragma once

//! Bare metal layer
namespace MTL {

class Mutex
{
public:
   Mutex();

   //! Block until lock gained
   void lock();

   //! Take lock an d return true else return false
   bool try_lock();

   void unlock();

private:
   unsigned handle{};
};

} // namespace MTL
