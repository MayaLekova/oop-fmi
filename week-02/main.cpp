#include <cstddef>
#include <cassert>
#include <iostream>

// специфично за Posix-базирани операционни системи
// (Linux, MacOS). Ползваме го заради `sleep`.
#include <unistd.h>

using std::cin, std::cout;

struct Point {
  double x;
  double y;
};

// Заделя масив в динамичната памет (на heap-а)
double* allocateDoubleArray(size_t length) {
  double* result = new double[length];
  // TODO: да проверим дали е успешно заделянето
  return result;
}

// Заделя масив от точки в динамичната памет
Point* allocatePointArray(size_t length) {
  Point* result = new Point[length];
  return result;
}

// Заделя масив от произволен тип T в динамичната памет.
// `T` наричаме "шаблонен тип" ли "template type".
template <typename T>
T* allocateArray(size_t length) {
  T* result = new T[length];
  return result;
}

int main() {
  size_t length = 0;
  cin >> length;
  assert(length > 0);

  // заделяме масив със специфичната функция
  Point* points_array = allocatePointArray(length);

  // заделяме масив с шаблонната функция
  Point* points_array_generic = allocateArray<Point>(length);

  sleep(20);

  return 0;
}