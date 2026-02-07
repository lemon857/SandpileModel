#pragma once

#include "utils.h"

struct BmpParams {
  int width;
  int height;
  Byte depth;
  Byte* color_palette;
  int color_palette_size;
  int count_colors_palette;
  int x_pix_per_metre;
  int y_pix_per_metre;
  
  BmpParams();
};

int BmpWrite(const char* file_name, Byte* raw_data, size_t raw_data_size, BmpParams& params);

int BmpWrite(const char* file_name, udmatrix64_t& matrix, BmpParams& params);

void WriteBytes(Byte* array, size_t from, size_t count, int source);