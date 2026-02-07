#pragma once

#include "utils.h"

enum class Style {
  classic,
  dark,
  purple,

};

void SandpileClear(udmatrix64_t& matrix);

bool SandpileTick(udmatrix64_t& matrix);
// return true, if do one and more actions
bool SandpileTick(udmatrix64_t& matrix, size_t& subticks, size_t& current_ticks, size_t max_ticks);

int SandpileFallLine(udmatrix64_t& matrix, index_t y);

int SandpileFallLine(udmatrix64_t& matrix, index_t y, size_t& subticks, size_t& current_ticks, size_t max_ticks);

void SandpileStart(ArgumentsStruct& arguments);

udmatrix64_t GenerateMatrixMultiply(long size);
