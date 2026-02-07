#include <cstring>
#include <cmath>
#include <filesystem>

#include "SandpileModel/sandpile.h"
#include "SandpileModel/utils.h"
#include "SandpileModel/dmatrix.h"
#include "SandpileModel/bmp_writer.h"

constexpr size_t kCountPaletteColors = 5;

// blue green red alfa

// light theme = 0
static Byte light_palette[] {
  0xFF, 0xFF, 0xFF, 0xFF, // white  = 0
  0x00, 0xFF, 0xF0, 0xFF, // green  = 1
  0xAB, 0x00, 0x5A, 0xFF, // purple = 2
  0x00, 0xFF, 0xFF, 0xFF, // yellow = 3
  0x00, 0x00, 0x00, 0xFF, // black >= 4
};

// dark theme = 1
static Byte dark_palette[] {
  0xAA, 0xAA, 0xAA, 0xFF, // gray   = 0
  0x00, 0xFF, 0xF0, 0xFF, // green  = 1
  0x06, 0x00, 0xFF, 0xFF, // purple = 2
  0xAB, 0x00, 0x5A, 0xFF, // purple = 3
  0xFF, 0xFF, 0xFF, 0xFF, // white >= 4
};

// purple theme = 2
static Byte purple_palette[] {
  0xAA, 0xAA, 0xAA, 0xFF, // gray   = 0
  0x00, 0xFF, 0xF0, 0xFF, // green  = 1
  0x06, 0x00, 0x0F, 0xFF, // purple = 2
  0xAB, 0x00, 0x5A, 0xFF, // purple = 3
  0xFF, 0xFF, 0xFF, 0xFF, // white >= 4
};

void SandpileClear(udmatrix64_t& matrix) {
  matrix.Clear();
}

int SandpileFallLine(udmatrix64_t& matrix, index_t y) {
  int result = 0;
  for (int i = -matrix[y].CountLeft(); i < matrix[y].CountRight(); ++i) {
    if (matrix(i, y).bv >= 4) {
      long add = matrix(i, y).bv >> 2;
      matrix(i, y) = matrix(i, y).bv % 4;
      matrix(i + 1, y).av += add;
      matrix(i - 1, y) += add;
      matrix(i, y + 1).av += add;
      matrix(i, y - 1) += add;
      result = 1;
    }
  }
  return result;
}

int SandpileFallLine(udmatrix64_t& matrix, index_t y, size_t& subticks, size_t& current_ticks, size_t max_ticks) {
  int result = 0;
  for (int i = -matrix[y].CountLeft(); i < matrix[y].CountRight(); ++i) {
    if (matrix(i, y).bv >= 4) {
      long add = matrix(i, y).bv >> 2;
      subticks += add;
      if (subticks + current_ticks > max_ticks && max_ticks != 0) {
        add = max_ticks - (max_ticks - subticks - current_ticks);
        subticks = max_ticks - current_ticks;
        current_ticks += subticks;
        matrix(i, y) = matrix(i, y).bv % 4;
        matrix(i + 1, y).av += add;
        matrix(i - 1, y) += add;
        matrix(i, y + 1).av += add;
        matrix(i, y - 1) += add;
        return 2;
      }
      matrix(i, y) = matrix(i, y).bv % 4;
      matrix(i + 1, y).av += add;
      matrix(i - 1, y) += add;
      matrix(i, y + 1).av += add;
      matrix(i, y - 1) += add;
      result = 1;
    }
  }
  return result;
}

bool SandpileTick(udmatrix64_t& matrix) {
  bool result = false;
  for (int i = -matrix.CountBottom(); i < matrix.CountTop(); ++i) {
    result |= SandpileFallLine(matrix, i);
  }
  for (int i = -matrix.CountBottom(); i < matrix.CountTop(); ++i) {
    for (int j = -matrix[i].CountLeft(); j < matrix[i].CountRight(); ++j) {
      matrix(j, i) = matrix(j, i).av;      
    }
  }
  return result;
}


bool SandpileTick(udmatrix64_t& matrix, size_t& subticks, size_t& current_ticks, size_t max_ticks) {
  bool result = false;
  bool is_break = false;
  for (int i = -matrix.CountBottom(); i < matrix.CountTop(); ++i) {
    int code = SandpileFallLine(matrix, i, subticks, current_ticks, max_ticks);
    if (code == 2) break;
    result |= code;
  }
  for (int i = -matrix.CountBottom(); i < matrix.CountTop(); ++i) {
    for (int j = -matrix[i].CountLeft(); j < matrix[i].CountRight(); ++j) {
      matrix(j, i) = matrix(j, i).av;
    }
  }
  return result;
}

void StashBmp(udmatrix64_t& matrix, BmpParams& params, const char* output_dir, size_t number) {
  char number_string[21];
  snprintf(number_string, 21, "%zu", number);
  if (output_dir != nullptr) {
    char* name_buffer = new char[strlen(output_dir) + strlen("/result.bmp") + 25];

    CStringConcatenation(name_buffer, output_dir, "/result", number_string, ".bmp");

    BmpWrite(name_buffer, matrix, params);
    // LOG("%s: %s, %d", "file write in", name_buffer, number);

    delete[] name_buffer;
  } else {
    char* name_buffer = new char[strlen("result.bmp") + 25];
   
    CStringConcatenation(name_buffer, "result", number_string, ".bmp");    

    BmpWrite(name_buffer, matrix, params);
    // LOG("%s: %s, %d", "file write in", name_buffer, number);

    delete[] name_buffer;
  }
}

udmatrix64_t GenerateMatrixMultiply(long size) {
  udmatrix64_t result;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      result(j, i) = i*j;
    }
  }
  return result;
}

void SandpileStart(ArgumentsStruct& arguments) {

  BmpParams params;

  switch (arguments.numder_style) {
  case 0:
  default:
    params.color_palette = light_palette;  
    break;
  case 1:
    params.color_palette = dark_palette;  
    break;
  case 2:
    params.color_palette = purple_palette;  
    break;
  }

  params.depth = 4;  
  params.color_palette_size = kCountPaletteColors * 4;
  params.count_colors_palette = kCountPaletteColors;
  params.x_pix_per_metre = 0x0B13;
  params.y_pix_per_metre = 0x0B13;

  udmatrix64_t tsv = ReadTSV(arguments.path_input_tsv);

  size_t ticks = 0;
  size_t subticks = 0;
  size_t current_sprint = 0;
  size_t current_number = 0;

  if (arguments.path_output_directory != nullptr) {
    std::filesystem::create_directory(arguments.path_output_directory);
  }

  while(SandpileTick(tsv, subticks, ticks, arguments.max_iterations_count) != false && (arguments.max_iterations_count == 0 || ticks < arguments.max_iterations_count)) {
    ticks += subticks;
    current_sprint += subticks;
    if(current_sprint >= arguments.save_frequency && arguments.save_frequency != 0) {
      current_sprint -= arguments.save_frequency;
      StashBmp(tsv, params, arguments.path_output_directory, current_number);
      ++current_number;
      current_sprint = 0;
    }
    subticks = 0;
  }

  // LOG_INFO("Ticks elapsed: {0}", ticks);

  StashBmp(tsv, params, arguments.path_output_directory, current_number);
}
