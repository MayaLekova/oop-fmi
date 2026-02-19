#include <iostream>

using std::cin, std::cout;

struct Point {
    double x;
    double y;
};

// Заделя масив на heap-а
Point* allocatePointArray(size_t length) {
    Point* result = new Point[length];
    return result;
}

void readPointArray(size_t length, Point* out_array) {
    Point p;
    for (int i = 0; i < length; ++i) {
        cin >> p.x >> p.y;
        out_array[i] = p;
    }
}

void printPointArray(size_t length, Point* dyn_array) {
    for (int i = 0; i < length; ++i) {
        cout << "(" << dyn_array[i].x
             << ", " << dyn_array[i].y << ") ";
    }
    cout << std::endl;
}

int main() {
    size_t points_count = 0;
    cin >> points_count;

    Point* points = allocatePointArray(points_count);

    readPointArray(points_count, points);

    cout << "X, Y coords: " << std::endl;
    printPointArray(points_count, points);
    // TODO: да имплементираме сканирането на Греъм (Graham scan)
    // за изчисляване на изпъкналата обвивка на точките в `points`.

    return 0;
}
