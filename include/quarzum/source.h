/*
 * Quarzum Compiler - source.h
 * Version 1.0, 06/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#pragma once
#include "string.h"

bool format(const char*& filename, const char* format);

const std::vector<char> getSource(const char*& filename);