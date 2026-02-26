#include <iostream>

struct Point {
  double x;
  double y;
};

// заделя масив от дробни числа
double* allocateDoubleArray(size_t length) {
  double* result = new double[length];
  return result;
}

// заделя масив от точки (Point)
Point* allocatePointArray(size_t length) {
  Point* result = new Point[length];
  return result;
}

// заделя масив от произволен тип T
template <typename T>
T* allocateArray(size_t length) {
  T* result = new T[length];
  return result;
}

// освобождава масив от произволен тип T
// и занулява указателя.
// еквивалентно с псевдоними би било:
// void deallocateArray(T*& array)
template <typename T>
void deallocateArray(T** array) {
  delete [] *array;
  *array = nullptr;
}

// `Cmp` приема функции със сигнатура:
// bool compare(Data, Data)
template <typename Data, typename Cmp>
size_t findMin(size_t len, Data* array, Cmp comparator) {
  if (len == 0) {
    return 0;
  }

  size_t cur_min_idx = 0;
  for (size_t i = 1; i < len; ++i) {
    Data cur_min = array[cur_min_idx];
    Data current = array[i];
    bool is_smaller = comparator(cur_min, current);
    if (!is_smaller) cur_min_idx = i;
  }

  return cur_min_idx;
}

bool compare(int x, int y) {
  return x < y;
}

int main() {
  int* numbers = allocateArray<int>(10);
  // 50, 45, ..., 5
  for(size_t i = 0; i < 10; ++i) {
    numbers[i] = (10 - i) * 5;
  }

  // използваме масив `numbers`
  size_t min_idx = findMin(10, numbers, compare);
  std::cout << "Minimal element is at index: " << min_idx << std::endl;
  
  // тук подаваме указател към масива `numbers`
  // т.е. параметърът е от тип int**
  deallocateArray(&numbers);

  // начин да изведем "суровия" указател charArray
  printf("charArray: %p\n", numbers);

  // можем да извеждаме и nullptr
  char* ptr = nullptr;
  printf("nullptr: %p\n", ptr);

  return 0;
}
