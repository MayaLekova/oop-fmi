// Да реализираме изпъкнала обвивка на множество от точки
// по зададени на входа координати в 2D (равнината).

#include <cstddef>
#include <cassert>
#include <iostream>

using std::cin, std::cout;

// Входни данни: в обичайния случай ще ги прочетем от файл.

// Ще реализираме динамичния еквивалент на следните статични
// масиви с входни данни:

// double input_coords_x[] = {1.5, 3, 3.8, 5, 5.8, 6, 7};
// double input_coords_y[] = {3, 6.5, 4.5, 2, 4, 5.8, 4.5}; 

double* allocateArray(size_t length) {
  double* result = new double[length];
  // можем да добавим евентуална проверка дали заделянето е успешно
  return result;
}

bool inputArray(size_t length, double* out_array) {
  if (!out_array) {
    return false;
  }

  double d;
  for (int i = 0; i < length; ++i) {
    cin >> d;
    out_array[i] = d;
    // или изразено с указателна аритметика:
    // *(out_array + i) = d;
  }
  return true;
}

void outputArray(size_t length, double* array) {
  for (int i = 0; i < length; ++i) {
    cout << array[i] << ", ";
  }
  cout << std::endl;
}

int main() {
  size_t length = 0;

  cin >> length;
  assert(length > 0);

  double* input_coords_x = allocateArray(length);
  double* input_coords_y = allocateArray(length);  

  cout << " === After initialization ===\n";
  cout << "X coords are: " << std::endl;
  outputArray(length, input_coords_x);
  cout << "Y coords are: " << std::endl;
  outputArray(length, input_coords_y);
  cout << "\n";

  inputArray(length, input_coords_x);
  inputArray(length, input_coords_y);

  cout << "\n=== After data read ===\n";
  cout << "X coords are: " << std::endl;
  outputArray(length, input_coords_x);
  cout << "Y coords are: " << std::endl;
  outputArray(length, input_coords_y);
  cout << "\n";

  return 0;
}

// ==========================
// 2-ри вариант: с използване на структура за точка.

constexpr size_t INPUT_SIZE = 7;
double input_points[INPUT_SIZE][2] = {
  {1.5, 3},
  {3, 6.5},
  {3.8, 4.5},
  {5, 2},
  {5.8, 4},
  {6, 5.8},
  {7, 4.5},
};

struct Point {
  double x;
  double y;
};

