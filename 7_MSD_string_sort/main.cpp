// Copyrights 2018 Dima Lipko

/*
 * Дан массив строк.
 * Количество строк не больше 10^5.
 * Отсортировать массив методом поразрядной сортировки MSD по символам.
 * Размер алфавита - 256 символов.
 * Последний символ строки = ‘\0’.
 * In    Out
 * ab   a
 * a    aa
 * aaa  aaa
 * aa   ab
 */

#include <iostream>
#include <cstring>
#include <cassert>

#define ALPHABET_SIZE 256

class Msd_string_sort {
 public:
    explicit Msd_string_sort(size_t N = 8): array(new std::string[N]), m_size(0), m_capacity(N) {}

    ~Msd_string_sort() {
            delete[] array;
    }

    Msd_string_sort(const Msd_string_sort&); // Конструктор копирования
    Msd_string_sort& operator=(const Msd_string_sort&); // Оператор присваивания копировнаием
    Msd_string_sort(Msd_string_sort &&) noexcept; // Конструктор перемещением
    Msd_string_sort& operator=(Msd_string_sort &&) noexcept; // Оператор присваивания перемещением

    void msd_sort(int32_t, int32_t = 0, int32_t = 0);

    // Сортировка заданной группы
    // По заданному байту
    void sort_group(int32_t, int, int *, int);

    void add(std::string &str);
    void print() const;

    size_t size() const {
        return m_size;
    }

 private:
    std::string *array;
    size_t m_size;
    size_t m_capacity;
    void memory_allocation(size_t);

    // Вычисляет границы групп в промежуточном массиве group_begin
    void count_group_start_index(int32_t *group_begin, size_t size);
};

Msd_string_sort::Msd_string_sort(const Msd_string_sort& other)
        :m_capacity(other.m_capacity), array(new std::string[other.m_capacity]),
         m_size(other.m_size)  {

    for (size_t i = 0; i < other.m_size; ++i) {
        array[i] = other.array[i];
    }
}

Msd_string_sort& Msd_string_sort::operator=(const Msd_string_sort& other) {
    if ( m_capacity < other.m_size)
        memory_allocation(other.m_capacity);

    for (size_t i = 0; i < other.m_size; ++i) {
        array[i] = other.array[i];
    }
    m_size = other.m_size;

    return *this;
}

Msd_string_sort::Msd_string_sort(Msd_string_sort&& other) noexcept
        :m_size(other.m_size), m_capacity(other.m_capacity), array(other.array) {
    other.array = nullptr;
}

Msd_string_sort& Msd_string_sort::operator=(Msd_string_sort && other) noexcept {
    delete[] array;
    m_size = other.m_size;
    array = other.array;
    other.array = nullptr;
    return *this;
}

void Msd_string_sort::add(std::string &str) {
    if (m_size + 1 == m_capacity)
        memory_allocation(m_capacity * 2);
    array[m_size++] = str;
}

void Msd_string_sort::memory_allocation(size_t capacity) {
    m_capacity = capacity;
    auto new_array = new std::string[m_capacity];
    assert(new_array != nullptr);
    for (size_t i = 0; i < m_size; ++i) {
        new_array[i] = array[i];
    }

    std::string *buf = array;
    array = new_array;
    delete [] buf;
}

// Посчитаем начальные позиции каждой группы
void Msd_string_sort::count_group_start_index(int32_t *group_begin,
                                              const size_t size) {
    int32_t start_index = 0;
    for(size_t i = 0; i < size; ++i) {
        int buf = group_begin[i];
        group_begin[i] = start_index;
        start_index += buf;
    }
}

void Msd_string_sort::sort_group(const int32_t begin, const int32_t size,
                                 int32_t *group_begin, const int32_t byte) {

    for(int i = 0; i < ALPHABET_SIZE; ++i)
        group_begin[i] = 0;

    // Посчитаем в group_begin[array[i][byte]] колличество вхождений
    // Элемента array[i][byte] в array
    for(int i = begin; i < begin + size; ++i)
        group_begin[array[i][byte]]++;

    // Посчитаем начальные позиции каждой группы
    count_group_start_index(group_begin, ALPHABET_SIZE);

    // Буффер для отсортированных значений
    // Переберем array. Элемент array[i]
    // Разместим в buffer в позицию группы group_begin[array[i][byte]]
    // Сдвинем текущую позицию группы
    auto buffer = new std::string[size];
    for(int i = begin; i < begin + size; ++i){
        buffer[group_begin[array[i][byte]]++] = array[i];
    }

    for(int i = 0; i < size; i++)
        array[i + begin] = buffer[i];

    delete[] buffer;
}

void Msd_string_sort::msd_sort(const int32_t size,
                               const int32_t begin, const int32_t byte) {
    if (size <= 1)
        return;

    // Промежуточный массив для подсчета в group_begin[i]
    // Количества вхождений элемента i в строку array[i]
    // Так же используется для подсчета начальных границ групп элементов
    auto * group_begin = new int32_t[ALPHABET_SIZE+ 1];
    group_begin[ALPHABET_SIZE] = size - 1;
    sort_group(begin, size, group_begin,  byte);

    for(int i = 1; i < ALPHABET_SIZE; i++)
        msd_sort(group_begin[i + 1] - group_begin[i],
                 begin + group_begin[i],  byte + 1);

    delete[] group_begin;
}

void Msd_string_sort::print() const {
    for (size_t i = 0; i < m_size; ++i)
        std::cout << array[i] << std::endl;
}

int32_t main() {
    Msd_string_sort str_list;
    while(!std::cin.eof()){
        std::string buf;
        std::cin >> buf;
        if (!buf.empty())
            str_list.add(buf);
    }
    str_list.msd_sort(str_list.size());
    str_list.print();
    return 0;
}