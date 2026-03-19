# Шаблони (templates)
Функция или структура, която работи не с променливи от някакъв дефиниран тип, а с абстрактни променливи.
```c++
#include <iostream>

template <class T>
T sum(const T& a, const T& b)
{
    return a + b;
}

int main()
{
    int a = 4;
    int b = 9;
    std::cout << sum<int>(a, b);

    double c = 3.14;
    double d = 4.5;
    std::cout << sum<double>(c,d);
}
```

Това, което се случва действително е, че по време на компилация, се генерират 2 различни функции с име sum - една функция, която
връща int и приема 2 константни референции от тип int и друга, която връща double и приема 2 константни референции от тип double. Такъв междинен код би изглеждал така:
```c++
#include <iostream>

template<class T>
T sum(const T& a, const T& b)
{
    return a + b;
}

template<>
int sum<int>(const int& a, const int& b)
{
    return a + b;
}

template<>
double sum<double>(const double& a, const double& b)
{
    return a + b;
}

int main()
{
    int a = 11;
    int b = 12;
    std::cout << sum<int>(a, b);

    double c = 3.14;
    double d = 2.81;
    std::cout << sum<double>(c, d);
}
```
Това, което видяхме горе, се нарича ***шаблонна специализация***  и можем да го използваме, когато искаме функциите ни да имат
общо поведение за почти всички типове данни, с малки изключения. Например, ако имаме функция за сортиране на масив с елементи от произволен тип, бихме 
използвали алгоритъм като selection sort, но можем да приложим друг по-оптимален алгоритъм за тип unsigned char, тъй като той има много малък
домейн от стойности (само 256) на брой и би било по-добре специално в този случай да използваме counting sort:
```c++
#include <iostream>

template <class T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <class T>
void sort(T* arr, unsigned size) {
    for (unsigned i = 0; i < size; i++) {
        unsigned minIndex = i;
        for (unsigned j = i + 1; j < size; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
        }
    }
}

template<>
void sort<unsigned char>(unsigned char* arr, unsigned size) {
    constexpr unsigned UCHAR_LIMIT = 256;

    unsigned occurrences[UCHAR_LIMIT]{ 0 };
    for (unsigned i = 0; i < size; i++) {
        occurrences[arr[i]]++;
    }

    for (unsigned i = 0; i < UCHAR_LIMIT - 1; i++) {
        occurrences[i + 1] += occurrences[i];
    }

    unsigned* result = new unsigned[size];
    for (int i = size - 1; i >= 0; i--) {
        result[--occurrences[arr[i]]] = arr[i];
    }

    for (unsigned i = 0; i < size; i++) {
        arr[i] = result[i];
    }
    delete[] result;
}

int main()
{
    int arr[] = { 5, 12, 9, 3, 7, 1 };
    sort(arr, 6);

    unsigned char arr2[] = "avgzytest";
    sort(arr2, 9);
}
```
