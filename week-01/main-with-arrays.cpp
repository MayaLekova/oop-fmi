#include <iostream>

using std::cin, std::cout;

// Заделя масив в динамичната памет (на heap-а)
double* allocateArray(size_t length) {
    double* result = new double[length];
    // да проверим дали е успешно заделяне
    return result;
}

// Прочита `length` числа от конзолата и ги записва
// в динамичен масив, в out_array
void readArray(size_t length, double* out_array) {
    double d;
    for (int i = 0; i < length; ++i) {
        cin >> d;
        // тук можем да валидираме координатите, например
        // дали са в определен интервал: [-1000, 1000] да речем
        out_array[i] = d;
    }
}

// Извежда динамично заделен масив с дължина `length` на конзолата
void printArray(size_t length, double* dyn_array) {
    for (int i = 0; i < length; ++i) {
        cout << dyn_array[i] << " ";
    }
    cout << std::endl;
}

int main() {
    size_t points_count = 0;
    cin >> points_count;

    double* x_coords = allocateArray(points_count);
    double* y_coords = allocateArray(points_count);

    readArray(points_count, x_coords);
    readArray(points_count, y_coords);

    cout << "X coords: " << std::endl;
    printArray(points_count, x_coords);
    cout << "Y coords: " << std::endl;
    printArray(points_count, y_coords);

    return 0;
}
