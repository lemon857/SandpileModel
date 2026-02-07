#pragma once

#include <cstdio>

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

constexpr float kExpandCoeff = 2.f;

using index_t = int64_t;

template<typename _T>
class DLine {
public:
  DLine()
    : data_left(nullptr)
    , data_right(nullptr)
    , size_left(0)
    , size_right(0) {

  }

  void Clear() {
    delete[] data_left;
    delete[] data_right;
  }

  _T& operator[](index_t index) {
    if (index >= size_right) {
      ExpandRight(index - size_right + 1);
    } else if (index < -size_left) {
      ExpandLeft(-index - size_left + 1);
    }

    if (index >= 0 && index < size_right) {
      return data_right[index];
    } else {
      return data_left[-index - 1];
    }
  }

  _T& AtService(index_t index, index_t& max_left, index_t& max_right) {
    if (index >= size_right) {
      ExpandRight(index - size_right + 1);
    } else if (index < -size_left) {
      ExpandLeft(-index - size_left + 1);
    }

    if (index >= 0 && index < size_right) {
      max_right = MAX(index + 1, max_right);
      return data_right[index];
    } else {
      max_left = MIN(index, max_left);
      return data_left[-index - 1];
    }
  }

  index_t CountLeft() const {
    return size_left;
  }

  index_t CountRight() const {
    return size_right;
  }
private:
  void ExpandRight(index_t require) {
    _T* temp = data_right;
    index_t new_size = static_cast<index_t>((size_right + require) * kExpandCoeff);
    data_right = new _T[new_size];
    for (int i = 0; i < size_right; ++i) {
      data_right[i] = temp[i];
    }
    for (int i = size_right; i < new_size; ++i) {
      data_right[i] = 0;
    }
    if (size_right != 0) delete[] temp;
    size_right = new_size;
  }

  void ExpandLeft(index_t require) {
    _T* temp = data_left;
    index_t new_size = static_cast<index_t>((size_left + require) * kExpandCoeff);
    data_left = new _T[new_size];
    for (int i = 0; i < size_left; ++i) {
      data_left[i] = temp[i];
    }
    for (int i = size_left; i < new_size; ++i) {
      data_left[i] = 0;
    }
    if (size_left != 0) delete[] temp;
    size_left = new_size;
  }

  _T* data_right;
  index_t size_right;

  _T* data_left;
  index_t size_left;

};

template<typename _T>
class DMatrix {
public:
  DMatrix() 
    : rows_top(nullptr)
    , rows_bottom(nullptr)
    , size_top(0) 
    , size_bottom(0)
    , max_right(0)
    , max_left(0)
    , max_top(0)
    , max_bottom(0) {

  }

  ~DMatrix() {
    for (int i = 0; i < size_top; ++i) {
      rows_top[i].Clear();
    }
    if (size_top != 0) delete[] rows_top;
    for (int i = 0; i < size_bottom; ++i) {
      rows_bottom[i].Clear();
    }
    if (size_bottom != 0) delete[] rows_bottom;
  }

  void Clear() {
    for (int i = 0; i < size_top; ++i) {
      rows_top[i].Clear();
    }
    if (size_top != 0) delete[] rows_top;
    for (int i = 0; i < size_bottom; ++i) {
      rows_bottom[i].Clear();
    }
    if (size_bottom != 0) delete[] rows_bottom;
    max_top = 0;
    max_bottom = 0;
    size_top = 0;
    size_bottom = 0;
    max_left = 0;
    max_right = 0;
  }

  DLine<_T>& operator[](index_t y) {
    if (y >= size_top) {
      max_top = MAX(y + 1, max_top);
      ExpandTop(y - size_top + 1);
    } else if (y < -size_bottom) {
      max_bottom = MIN(y, max_bottom);
      ExpandBottom(-y - size_bottom);
    }
    if (y < size_top && y >= 0) {
      return rows_top[y];
    } else {
      return rows_bottom[-y - 1];
    }
  }

  const _T& At(index_t x, index_t y) {
    if (y >= size_top) {
      ExpandTop(y - size_top + 1);
    } else if (y < -size_bottom) {
      ExpandBottom(-y - size_bottom);
    }
    if (y < size_top && y >= 0) {
      max_top = MAX(y + 1, max_top);
      _T& current = rows_top[y].AtService(x, max_left, max_right);
      return current;
    } else {
      max_bottom = MIN(y, max_bottom);
      _T& current = rows_bottom[-y - 1].AtService(x, max_left, max_right);
      return current;
    }
  }

   _T& operator()(index_t x, index_t y) {
    if (y >= size_top) {
      ExpandTop(y - size_top + 1);
    } else if (y < -size_bottom) {
      ExpandBottom(-y - size_bottom);
    }
    if (y < size_top && y >= 0) {
      max_top = MAX(y + 1, max_top);
      _T& current = rows_top[y].AtService(x, max_left, max_right);
      return current;
    } else {
      max_bottom = MIN(y, max_bottom);
      _T& current = rows_bottom[-y - 1].AtService(x, max_left, max_right);
      return current;
    }
  }

  index_t CountTop() const {
    return max_top;
  }

  index_t CountBottom() const {
    return -max_bottom;
  }

  index_t MaxRightLine() const {
    return max_right;
  }
  index_t MaxLeftLine() const {
    return -max_left;
  }
private:
  
  void ExpandTop(index_t require) {
    DLine<_T>* temp = rows_top;
    index_t new_size = static_cast<index_t>((size_top + require) * kExpandCoeff);
    rows_top = new DLine<_T>[new_size];
    for (int i = 0; i < size_top; ++i) {
      rows_top[i] = temp[i];
    }
    if (size_top != 0) delete[] temp;
    size_top = new_size;
  }

  void ExpandBottom(index_t require) {
    DLine<_T>* temp = rows_bottom;
    index_t new_size = static_cast<index_t>((size_bottom + require) * kExpandCoeff);
    rows_bottom = new DLine<_T>[new_size];
    for (int i = 0; i < size_bottom; ++i) {
      rows_bottom[i] = temp[i];
    }
    if (size_bottom != 0) delete[] temp;
    size_bottom = new_size;
  }

  DLine<_T>* rows_top;
  DLine<_T>* rows_bottom;

  index_t size_top;
  index_t size_bottom;

  index_t max_left;
  index_t max_right;  
  index_t max_top;
  index_t max_bottom; 
};