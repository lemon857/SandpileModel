#include "SandpileModel/utils.h"

#include <cmath>
#include <cstring>
#include <fstream>

// 2 * max_int16_numbers + max_uint64_numbers + 2 '\t'
const size_t kMaxBufferSize = static_cast<size_t>(2.0 * log10(INT16_MAX) + log10(UINT64_MAX)) + 10;

int FindInCString(const char* string, char symbol) {
  int i = 0;
  while (string[i] != symbol && string[i] != '\0') {
    ++i;
  }
  if (string[i] == symbol) return i;
  return -1;
}

int FindLastInCString(const char* string, char symbol) {
  int i = 0;
  while (string[i] != '\0') {
    ++i;
  }
  while (string[i] != symbol && i >= 0) {
    --i;
  }

  if (string[i] == symbol) return i;
  return -1;
}

long long GetNumberFromEndTo(const char* string, const char symbol) {
  long long result = 0;
  int i = 0;
  while (string[i] != '\0') {
    ++i;
  }
  --i;
  int j = 0;
  while (i >= 0 && string[i] != symbol) {
    result += (string[i] - '0') * std::pow(10, j);
    ++j;
    --i;
  }
  return result;
}

long long GetNumberFromTo(const char* string, long from, long to) {
  long long result = 0;
  char sign = 1;
  if (string[from] == '-') {
    sign = -1;
    ++from;
  }
  for (int i = to - 1; i >= from; --i) {
    result += (string[i] - '0') * std::pow(10, to - i - 1);
  }
  return result * sign;
}

long GetCStringLenght(const char* string) {
 long i = 0;
  while (string[i] != '\0') {
    ++i;
  }
  return i;
}

udmatrix64_t ReadTSV(const char* path) {
  udmatrix64_t result;
  
  int16_t x;
  int16_t y;
  uint64_t count;

  std::ifstream f;

  f.open(path, std::ios::in);

  if (!f.is_open()) {
    // ERROR_LOG("open file: %s", path);
    return result;
  }
  char buffer[kMaxBufferSize];
  while (!f.eof()) {
    f.getline(buffer, kMaxBufferSize);

    int tab_start = FindInCString(buffer, '\t');
    int tab_end = FindLastInCString(buffer, '\t');
    int lenght = strlen(buffer);

    if (lenght <= 0) continue;

    x = GetNumberFromTo(buffer, 0, tab_start);
    y = GetNumberFromTo(buffer, tab_start + 1, tab_end);
    count = GetNumberFromTo(buffer, tab_end + 1, lenght);

    result(x, y) = count;
  }

  return result;
}

// void PrintMatrix(udmatrix64_t& matrix) {
//   printf("top: %d\tbottom: %d\t left: %d\t right: %d\n", matrix.CountTop(), matrix.CountBottom(), matrix.MaxLeftLine(), matrix.MaxRightLine());
//   for (int i = -matrix.CountBottom(); i < matrix.CountTop(); ++i) {
//     for (int j = -matrix.MaxLeftLine(); j < matrix.MaxRightLine(); ++j) {
//       if (j >= -matrix[i].CountLeft() && j <= matrix[i].CountRight()) {
//         printf("%d\t", matrix(j, i));
//       } else {
//         printf("+\t");
//       }
//     }
//     printf("\n");
//   }
// }

void CStringConcatenation(char* out_buffer, const char* first, const char* second) {
  int i = 0;
  int max = 0;
  while (first[i] != '\0') {
    out_buffer[i] = first[i];
    ++i;
  }
  max = i;
  while (second[i - max] != '\0') {
    out_buffer[i] = second[i - max];
    ++i;
  }
  out_buffer[i] = '\0';
}

void CStringConcatenation(char* out_buffer, const char* first, const char* second, const char* third) {
  int i = 0;
  int max = 0;
  while (first[i] != '\0') {
    out_buffer[i] = first[i];
    ++i;
  }
  max = i;
  while (second[i - max] != '\0') {
    out_buffer[i] = second[i - max];
    ++i;
  }
  max = i;

  while (third[i - max] != '\0') {
    out_buffer[i] = third[i - max];
    ++i;
  }
  out_buffer[i] = '\0';
}

void CStringConcatenation(char* out_buffer, const char* first, const char* second, const char* third, const char* fourth) {
  int i = 0;
  int max = 0;
  while (first[i] != '\0') {
    out_buffer[i] = first[i];
    ++i;
  }
  max = i;
  while (second[i - max] != '\0') {
    out_buffer[i] = second[i - max];
    ++i;
  }
  max = i;

  while (third[i - max] != '\0') {
    out_buffer[i] = third[i - max];
    ++i;
  }
  max = i;

  while (fourth[i - max] != '\0') {
    out_buffer[i] = fourth[i - max];
    ++i;
  }
  out_buffer[i] = '\0';
}
