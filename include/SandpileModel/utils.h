#pragma once

#include <cstdio>
#include <cstddef>
#include <cstdint>

#include "dmatrix.h"

struct Pixel {
  // before value
  uint64_t bv;
  // after value
  uint64_t av;
  void operator=(uint64_t&& value) {
    bv = value;
    av = value;
  }
  void operator=(uint64_t& value) {
    bv = value;
    av = value;
  }
  void operator+=(uint64_t&& value) {
    bv += value;
    av += value;
  }
};

struct ArgumentsStruct
{
  long max_iterations_count;
  long save_frequency;

  const char* path_input_tsv;
  const char* path_output_directory;

  int numder_style;
};

using udmatrix64_t = DMatrix<Pixel>;

using Byte = uint8_t;

int FindInCString(const char* string, char symbol);

int FindLastInCString(const char* string, char symbol);

long long GetNumberFromEndTo(const char* string, const char symbol);

long long GetNumberFromTo(const char* string, long from, long to);

void CStringConcatenation(char* out_buffer, const char* first, const char* second);

void CStringConcatenation(char* out_buffer, const char* first, const char* second, const char* third);

void CStringConcatenation(char* out_buffer, const char* first, const char* second, const char* third, const char* fourth);

udmatrix64_t ReadTSV(const char* path);

// void PrintMatrix(udmatrix64_t& matrix);
