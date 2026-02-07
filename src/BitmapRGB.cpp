#include "SandpileApplication/BitmapRGB.h"
#include <cstring>

BitmapRGB::BitmapRGB(std::size_t width, std::size_t height)
  : _data(new unsigned char[width * height * 3])
  , _width(width)
  , _height(height) {}

BitmapRGB::BitmapRGB(std::size_t width, std::size_t height, BitmapRGB::ColorRGB value)
  : _data(new unsigned char[width * height * 3])
  , _width(width)
  , _height(height) {
  for (size_t i = 0; i < width * height * 3; ++i) {
    if (i % 3 == 0) {
      _data[i] = value.r;
    } else if (i % 3 == 1) {
      _data[i] = value.g;
    } else {
      _data[i] = value.b;
    }
  }
}

BitmapRGB::~BitmapRGB() {
  delete[] _data;
}

void BitmapRGB::set_color(std::size_t x, std::size_t y, BitmapRGB::ColorRGB color) {
  _data[y * _width * 3 + x * 3] = color.r;
  _data[y * _width * 3 + x * 3 + 1] = color.g;
  _data[y * _width * 3 + x * 3 + 2] = color.b;
}

const unsigned char* BitmapRGB::get_data() const {
  return _data;
}
