//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "MIDI/Const.h"

namespace Novation {

namespace LaunchKey {

// NOTE: These channel allocations have been
//       setup via the customization tool

inline constexpr unsigned TOP1 = 2;
inline constexpr unsigned BTM1 = 10;
inline constexpr unsigned LVL1 = 20;

inline constexpr unsigned TOP2 = 3;
inline constexpr unsigned BTM2 = 11;
inline constexpr unsigned LVL2 = 21;

inline constexpr unsigned TOP3 = 4;
inline constexpr unsigned BTM3 = 12;
inline constexpr unsigned LVL3 = 22;

inline constexpr unsigned TOP4 = 5;
inline constexpr unsigned BTM4 = 13;
inline constexpr unsigned LVL4 = 23;

inline constexpr unsigned TOP5 = 6;
inline constexpr unsigned BTM5 = 14;
inline constexpr unsigned LVL5 = 24;

inline constexpr unsigned TOP6 = 7;
inline constexpr unsigned BTM6 = 15;
inline constexpr unsigned LVL6 = 25;

inline constexpr unsigned TOP7 = 8;
inline constexpr unsigned BTM7 = 16;
inline constexpr unsigned LVL7 = 26;

inline constexpr unsigned TOP8 = 9;
inline constexpr unsigned BTM8 = 17;
inline constexpr unsigned LVL8 = 27;

inline constexpr unsigned LVL9 = 28;

inline constexpr unsigned MASTER = LVL9;

extern const uint8_t map[29];

} // LaunchKey

} // Novation
