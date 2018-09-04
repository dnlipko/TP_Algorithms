// Copyright 2018 Dmitrii Lipko

/*
 * Во всех задачах данного раздела необходимо реализовать
 * и использовать сортировку слиянием.
 * Общее время работы алгоритма O(n log n).
 * 5_1. Реклама.
 * В супермаркете решили оптимизировать показ рекламы.
 * Известно расписание прихода и ухода покупателей (два целых числа).
 * Каждому покупателю необходимо показать минимум 2 рекламы.
 * Рекламу можно транслировать только в целочисленные моменты времени.
 * Покупатель может видеть рекламу от момента прихода до момента ухода из магазина.
 * В каждый момент времени может показываться только одна реклама.
 * Считается, что реклама показывается мгновенно.
 * Если реклама показывается в момент ухода или прихода, то считается,
 * что посетитель успел её посмотреть.
 * Требуется определить минимальное число показов рекламы.
 * In
 * 5
 * 1 10
 * 10 12
 * 1 10
 * 1 10
 * 23 24
 *
 * Out
 * 5
 */

#include <iostream>
#include <cstring>

// Покупатель(время прихода, время ухода, колличество показанной рекламы)
struct Customer {
    int32_t in_time;
    int32_t out_time;
    int32_t adverts;

    Customer(int32_t time_in, int32_t time_out):
            in_time(time_in), out_time(time_out), adverts(0) {}

    Customer():in_time(0), out_time(0), adverts(0) {}

    bool operator<(const Customer&) const;
};

// Сравнивает покупателей по времени ухода
// При равном по времени прихода
bool Customer::operator<(const Customer &other) const {
    if (out_time < other.out_time)
        return true;
    if (out_time == other.out_time)
        return in_time > other.in_time;
    return false;
}

// Список посетителей
template <typename T>
class Customers_queue {
 public:
    explicit Customers_queue(int32_t N): array(new T[N]), m_size(0) {}

    ~Customers_queue() {
        delete[] array;
    }
    Customers_queue(const Customers_queue&); // Кнструктор копирования
    Customers_queue& operator=(const Customers_queue&); // Ооператор присваивания копированием
    Customers_queue(Customers_queue&&) noexcept; // Конструктор перемещения
    Customers_queue& operator=(Customers_queue&&) noexcept; // Оператор присваивания перемещением

    void add(T);
    void merge_sort(int32_t, int32_t);
    int32_t advertisement_counter();
    size_t size() const {
        return m_size;
    }

 private:
    T *array;
    size_t m_size;
    void merge(int32_t, int32_t, int32_t);
    void adverts_inc(int32_t, int32_t);
};

// Конструктор копирования
template <typename T>
Customers_queue<T>::Customers_queue(const Customers_queue& other):
        m_size(other.m_size), array(new T[m_size]) {
    for (size_t i = 0; i < m_size; ++i)
        array[i] = other.array[i];
}

// Конструктор перемещения
template <typename T>
Customers_queue<T>::Customers_queue(Customers_queue&& other) noexcept:
        array(other.array), m_size(other.m_size) {
    other.array = nullptr;
}


// Оператор присваивания копированием
template <typename T>
Customers_queue<T>& Customers_queue<T>::operator=(const Customers_queue& other) {
    Customers_queue buf(other);
    *this = std::move(buf);
    return *this;
}

// Опрератор присваивания перемещением
template <typename T>
Customers_queue<T>& Customers_queue<T>::operator=(Customers_queue&& other) noexcept {
    if (this == &other)
        return *this;

    delete[] array;
    array = other.array;
    other.array = nullptr;
    m_size = other.m_size;
    return *this;
}

// Добавление посетителя
template <typename T>
void Customers_queue<T>::add(T customer) {
    array[m_size++] = customer;
}

// Сортировка посетителей по времени ухода( при равенстве, прихода)
template <typename T>
void Customers_queue<T>::merge_sort(int32_t begin, int32_t end) {
    // Список из одного элемента отсортирован
    if (end - begin == 1)
        return;

    // Индекс серединного элемента
    int32_t middle =  begin + (end - begin) / 2;

    // Сортируем половинки массива
    merge_sort(begin,  middle);
    merge_sort(middle, end);

    // Составляем из двух отсортированных половинок
    // Целый отсортированный массив
    merge(begin, middle, end);
}

// Метод слияния массивов
template <typename T>
void Customers_queue<T>::merge(int32_t left, int32_t middle, int32_t right) {
    int32_t it_left = 0, it_right = 0;

    // Буфферный массив для хранения первой половины массива
    auto *buf = new T[middle - left];
    memcpy(buf, array + left, sizeof(T) * (middle - left));

    // Сливаем буфрный массив и исходный массив с индекса middle
    //  В исходный массив
    while (left + it_left < middle && middle + it_right < right) {
        if (buf[it_left] < array[middle + it_right]) {
            array[left + it_left + it_right] = buf[it_left];
            it_left++;
        } else {
            array[left + it_left + it_right] = array[middle + it_right];
            it_right++;
        }
    }
    // Если остались элементы в буферном массиве
    // Перенсим их в исходный массив
    for ( ; left + it_left < middle; ++it_left)
        array[left + it_left + it_right] = buf[it_left];

    delete[] buf;
}

// Увеличивает счетчик показаной рекламы для
// Всех посетителей пришедших в заданный промежуток времени
template <typename T>
void Customers_queue<T>::adverts_inc(int32_t customer, int32_t time_moment) {
    for (size_t i = customer; i < size() &&
            array[i].in_time <= time_moment; ++i)
        ++array[i].adverts;
}

// Метод считающий колличество необходимой для показа рекламы
template <typename T>
int32_t Customers_queue<T>::advertisement_counter() {
    int32_t ads = 0;
    for(size_t i = 0; i < size(); i++) {
        if(array[i].adverts == 0) {
            adverts_inc(i, array[i].out_time - 1);
            ads++;
        }
        if(array[i].adverts == 1) {
            adverts_inc(i, array[i].out_time);
            ads++;
        }
    }
    return ads;
}

int32_t main() {
    size_t N = 0;
    std::cin >> N;
    Customers_queue<Customer> customers_queue(N);

    for (size_t i = 0; i < N; ++i) {
        int32_t in_time =0, out_time = 0;
        std::cin >> in_time >> out_time;

        Customer new_one(in_time, out_time);
        customers_queue.add(new_one);
    }

    customers_queue.merge_sort(0, N);
    std::cout << customers_queue.advertisement_counter();
    return 0;
}
