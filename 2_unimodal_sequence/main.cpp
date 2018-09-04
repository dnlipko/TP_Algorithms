// Copyrights 2018 Dima Lipko

/*
 * Дан массив целых чисел А[0..n-1].
 * Известно, что на интервале [0, m] значения массива строго возрастают,
 * а на интервале [m, n-1] строго убывают.
 * Найти m за O( log m ).
 * 2 ≤ n ≤ 10000.
 *
 * Ввод
 * 10
 * 1 2 3 4 5 6 7 6 5 4
 *
 * Вывод
 * 6
*/

#include <iostream>
#include "ctime"

// Бинарный поиск
int bin_search(int const *array, int left, int right) {
    while (true) {
        int mid = left + (right - left) / 2;

        if (array[mid] > array[mid + 1] &&
                array[mid] > array[mid - 1])
            return mid;

        if (array[mid] > array[mid + 1])
            right = mid;
        else
            left = mid + 1;
    }
}

// Выделение искомого интервала
// Прохождением по индексам равным степеням двойки
int find_m_log(int const *array, int begin, int end) {

    // Размер массива = 2
    if (end + 1 == 2)
        if (array[0] < array[1])
            return 1;
        else
            return 0;

    // Строго убывающая последовательность
    if (array[0] > array[1])
        return 0;

    // Строго возрастающая последовательность
    if (array[end - 1] < array[end])
        return end;

    int runner = 1;
    while (runner < end) {
        if (array[runner - 1] > array[ begin + runner]) {
            // runner > m, перепрыгнули m
            // begin + runner/2 < m < runner
            return bin_search(array, runner / 2, runner);
        } else
            // runner - 1 < runner
        if (array[begin + runner] > array[begin + runner + 1]) {
            //runner - 1 < runner < runner + 1
            return begin + runner;
        } else
            runner *= 2;
    }
    // runner/2 < m < end
    return(bin_search(array, runner / 2, end));
}

int main() {
    int N = 0;
    std::cin >> N;
    int * array = new int[N];
    for (int i = 0; i < N; ++i)
        std::cin >> array[i];
    std::cout << find_m_log(array, 0, N - 1) << std::endl;
    delete [] array;
    return 0;
}
