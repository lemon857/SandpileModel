#include "SandpileModel/bmp_writer.h"

#include <fstream>
#include <cmath>

constexpr size_t kHeaderSize = 0x36;

BmpParams::BmpParams() 
  : width(0)
  , height(0)
  , depth(0)
  , color_palette(nullptr)
  , color_palette_size(0)
  , count_colors_palette(0)
  , x_pix_per_metre(0)
  , y_pix_per_metre(0) {

}

int BmpWrite(const char* file_name, Byte* raw_data, size_t raw_data_size, BmpParams& params) {

  Byte header[kHeaderSize];

  for (int i = 0; i < kHeaderSize; ++i) {
    header[i] = 0x00;
  }

  // Write signature and service value
  header[0x00] = 'B';
  header[0x01] = 'M';
  header[0x0E] = 0x28;
  header[0x1A] = 0x01;

  // File size
  WriteBytes(header, 0x02, 4, kHeaderSize + params.color_palette_size + raw_data_size);
  // Offset to pixels data
  WriteBytes(header, 0x0A, 4, kHeaderSize + params.color_palette_size);
  // Width
  WriteBytes(header, 0x12, 4, params.width);
  // Height
  WriteBytes(header, 0x16, 4, params.height);
  // Depth
  WriteBytes(header, 0x1C, 1, params.depth);
  // X pixels per metre
  WriteBytes(header, 0x26, 1, params.x_pix_per_metre);
  // Y pixels per metre
  WriteBytes(header, 0x2A, 1, params.y_pix_per_metre);
  // Color palette size
  WriteBytes(header, 0x2E, 1, params.count_colors_palette);

  std::fstream f;
  f.open(file_name, std::ios::binary | std::ios::out);
  if (!f.is_open()) {
    // ERROR_LOG("open file: %s", file_name);
    return -1;
  }

  f.write(reinterpret_cast<char*>(header), kHeaderSize);
  if (params.color_palette_size != 0) {
    f.write(reinterpret_cast<char*>(params.color_palette), params.color_palette_size);
  }
  f.write(reinterpret_cast<char*>(raw_data), raw_data_size);

  f.close();
  return 0;
}

void WriteBytes(Byte* array, size_t from, size_t count, int source) {
  for (int i = from; i < count + from; ++i) {
    array[i] = source & 0xFF;
    source >>= 8;
  }
}

int BmpWrite(const char* file_name, udmatrix64_t& matrix, BmpParams& params) {
  Byte header[kHeaderSize];

  for (int i = 0; i < kHeaderSize; ++i) {
    header[i] = 0x00;
  }

  // Write signature and service value
  header[0x00] = 'B';
  header[0x01] = 'M';
  header[0x0E] = 0x28;
  header[0x1A] = 0x01;

  index_t width = matrix.MaxLeftLine() + matrix.MaxRightLine();
  index_t height = matrix.CountBottom() + matrix.CountTop();

  params.width = width;
  params.height = height;

  // File size
  WriteBytes(header, 0x02, 4, kHeaderSize + params.color_palette_size + (width + width % 8) * height);
  // Offset to pixels data
  WriteBytes(header, 0x0A, 4, kHeaderSize + params.color_palette_size);
  // Width
  WriteBytes(header, 0x12, 4, width);
  // Height
  WriteBytes(header, 0x16, 4, height);
  // Depth
  WriteBytes(header, 0x1C, 1, params.depth);
  // X pixels per metre
  WriteBytes(header, 0x26, 1, params.x_pix_per_metre);
  // Y pixels per metre
  WriteBytes(header, 0x2A, 1, params.y_pix_per_metre);
  // Color palette size
  WriteBytes(header, 0x2E, 1, params.count_colors_palette);

  std::fstream f;
  f.open(file_name, std::ios::binary | std::ios::out);
  if (!f.is_open()) {
    // ERROR_LOG("open file: %s", file_name);
    return -1;
  }

  f.write(reinterpret_cast<char*>(header), kHeaderSize);
  if (params.color_palette_size != 0) {
    f.write(reinterpret_cast<char*>(params.color_palette), params.color_palette_size);
  }
  
  uint8_t byte = 0;
  uint8_t null_byte = 0;
  uint8_t start_remainder = matrix.MaxLeftLine() % 2;
  double add_require_start_line = 8 - (width % 8);
  double add_require = add_require_start_line;

  index_t left = matrix.MaxLeftLine();
  index_t right = matrix.MaxRightLine();

  for (int i = -matrix.CountBottom(); i < matrix.CountTop(); ++i) {
    add_require = add_require_start_line;
    for (int j = -left; j < right; ++j) {
      if (j >= -matrix[i].CountLeft() && j <= matrix[i].CountRight()) {
        if (abs(j) % 2 == start_remainder) {
          byte |= (matrix(j, i).bv < 4 ? matrix(j, i).bv : 4) << 4;
        } else {
          byte |= (matrix(j, i).bv < 4 ? matrix(j, i).bv : 4);
          f.write(reinterpret_cast<char*>(&byte), 1);
          byte = 0;
        }
      } else {
        if (abs(j) % 2 != start_remainder) {
          f.write(reinterpret_cast<char*>(&null_byte), 1);
        }
      }
    }

    if (byte != 0) {
      f.write(reinterpret_cast<char*>(&byte), 1);
      byte = 0;
      --add_require;
    }
    for (int j = 0; j < ceil(static_cast<double>(add_require) / 2.0); ++j) {
      f.write(reinterpret_cast<char*>(&byte), 1);      
    }
  }
  return 0;
}
