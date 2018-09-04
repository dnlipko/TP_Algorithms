// Copyrights 2018 Dima Lipko

/*
 * Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
 * Требуется найти k-ю порядковую статистику.
 * т.е. напечатать число, которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
 * Напишите нерекурсивный алгоритм.
 * Требования к дополнительной памяти: O(n).
 * Требуемое среднее время работы: O(n).
 * Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
 * Реализуйте стратегию выбора опорного элемента “случайный элемент”.
 * Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.
 *
 * Ввод	                    Вывод
 * 10 4
 * 1 2 3 4 5 6 7 8 9 10     5
 *
 * 10 0
 * 3 6 5 7 2 9 8 10 4 1     1
 *
 * 10 9
 * 0 0 0 0 0 0 0 0 0 1      1
 */

#include <iostream>
#include <cstring>
#include <ctime>

// Класс содержащий массив и его длину
// Реализован метод поиска порядковой статистики
class Order_array {
 public:
    explicit Order_array(int32_t N): array(new int32_t[N]), m_size(0) {}

    ~Order_array() {
        delete[] array;
    }

    // Конструктор копировнаия
    Order_array(const Order_array &);

    // Конструктор перемещения
    Order_array(Order_array&&) noexcept;

    // Оператор присваивания копированием
    Order_array& operator=(const Order_array&);
    
    // Оператор присваивания перемещением
    Order_array& operator=(Order_array&&) noexcept;

    void add(int32_t value) {
        array[m_size++] = value;
    }
    
    // Функция поиска порядковой статистики
    int32_t order_statistic(int32_t);

    size_t size() {
        return m_size;
    }

 private:
    int32_t *array;
    size_t m_size;
    
    // Меняет местами элементы массива по индексу
    void swap(int32_t, int32_t);
    
    // Разделяет массив на две части
    // Левая меньше пивота
    // Правая больше пивота
    int32_t partition(int32_t, int32_t);
};

Order_array::Order_array(const Order_array& other)
        :array(new int32_t[other.m_size]), m_size(other.m_size) {
    memcpy(array, other.array, sizeof(int32_t) * m_size);
}

Order_array::Order_array(Order_array &&other) noexcept
        :array(other.array), m_size(other.m_size) {
    other.array = nullptr;
}

Order_array& Order_array::operator=(const Order_array& other) {
    delete[] array;
    array = new int32_t[other.m_size];
    memcpy(array, other.array, sizeof(int32_t) * other.m_size);
    m_size = other.m_size;
    return *this;
}

Order_array& Order_array::operator=(Order_array&& other) noexcept {
    delete[] array;
    array = other.array;
    other.array = nullptr;
    m_size= other.m_size;
    return *this;
}

void Order_array::swap(int32_t index1, int32_t index2) {
    int32_t buf = array[index1];
    array[index1] = array[index2];
    array[index2] = buf;
}

int32_t Order_array::partition(int32_t begin, int32_t end) {
    // Пивот выбирается случайным образом
    srand(time(NULL));
    int32_t pivot_index = rand() % (end - begin + 1) + begin;
    int32_t pivot = array[pivot_index];
    swap(pivot_index, end);
    int32_t i = begin, j = begin;
    while (i <= end && array[i] <= pivot)
        i++;

    if (i >= end)
        return end;
    j = i + 1;

    while (j < end) {
        if (array[j] <= pivot && j < end)
            swap(i++, j);
        j++;
    }

    swap(i, end);
    return i;
}

int32_t  Order_array::order_statistic(int32_t index) {
    int32_t pivot_index = -1;
    int32_t begin = 0, end = m_size - 1;
    while (pivot_index != index) {
        pivot_index = partition(begin, end);
        if (pivot_index < index) {
            begin = pivot_index + 1;
        } else {
            end = pivot_index -1;
        }
    }
    return array[index];
}

int main() {
    int32_t k = 0;
    size_t N = 0;
    std::cin >> N >> k;
    Order_array array(N);
    for (size_t i = 0; i < N; ++i) {
        int32_t buf = 0;
        std::cin >> buf;
        array.add(buf);
    }
    std::cout << array.order_statistic(k);
    return 0;
}
