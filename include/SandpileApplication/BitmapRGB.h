#pragma once

#include <cstddef>

class BitmapRGB {
private:
  unsigned char* _data;
  std::size_t _width;
  std::size_t _height;

public:
  struct ColorRGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
  };

  BitmapRGB(std::size_t width, std::size_t height);
  BitmapRGB(std::size_t width, std::size_t height, ColorRGB value);

  BitmapRGB(const BitmapRGB&) = delete;
  BitmapRGB& operator=(const BitmapRGB&) = delete;

  ~BitmapRGB();

  void set_color(std::size_t x, std::size_t y, ColorRGB color);

  const unsigned char* get_data() const;
};
