//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdio>

namespace NDL {

class Model;

bool   error(const char* format, ...);
bool   warn(const char* format, ...);

FILE*  openForWrite(const Model* model_, const char* dir_, const char* extension_);
Model* readXML(const char* filename_);

} // namespace NDL
