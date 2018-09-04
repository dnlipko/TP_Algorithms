// Copyright 2018 Dmitrii Lipko

/*
    Реализовать дек с динамическим зацикленным буфером.

    Обрабатывать команды push * и pop *.

    Формат ввода
    В первой строке количество команд n. n ≤ 1000000.

    Каждая команда задаётся как 2 целых числа: a b.

    a = 1 - push front
    a = 2 - pop front
    a = 3 - push back
    a = 4 - pop back
    Если дана команда pop *, то число b - ожидаемое значение.
    Если команда pop * вызвана для пустой структуры данных,
    то ожидается “-1”.
 */


#include <iostream>
#include <cstring>
#include "cassert"

// Дэк с динамическим буфером
template <typename T>
class Deque {
 public:
    Deque(): m_buf(new T[4]), m_head(0), m_tail(0), m_capacity(4) {}

    ~Deque() {
        delete[]m_buf;
    }

    // Правило пяти

    //удаляет и возвращает элемент с конца дэка
    T pop_back();
    //удаляет и возвращает элемент с начала дэка
    T pop_front();

    //добавляет элемент в конец дэка
    void push_back(T const value);
    //добавляет элемент в начало дэка
    void push_front(T const value);

    //проверяет наличие элементов  в дэке
    bool empty() const {
        return m_tail == m_head;
    };
    //проверяет заполненость дэка
    bool full() const {
       return m_tail + 1 == m_head
        || (m_tail + 1 == m_capacity && m_head ==0);
    }

 private:
    T *m_buf;
    int32_t m_head;
    int32_t m_tail;
    size_t m_capacity;

    //увеличение размера дэка в 2 раза
    void memory_allocation();
};


template <typename T>
void Deque<T>::memory_allocation() {
    auto *new_buf = new T[m_capacity * 2];
    assert(new_buf != nullptr);

    if (m_head > m_tail) {
        memcpy(new_buf + m_head, m_buf + m_head,
               (m_capacity - m_head) * sizeof(T));
        memcpy(new_buf + m_capacity, m_buf, m_tail * sizeof(T));
        m_tail += m_capacity;
    } else {
        memcpy(new_buf, m_buf, (m_tail - m_head) * sizeof(T));
    }

    delete [] m_buf;
    m_buf = new_buf;
    m_capacity *= 2;
};


template <typename T>
void Deque<T>::push_back(T const value) {
    if (full())
        memory_allocation();

    m_buf[ m_tail++ ] = value;
    m_tail %= m_capacity;
}

template <typename T>
void Deque<T>::push_front(T const value) {
    if (full())
        memory_allocation();

    if (empty()) {
        m_buf[m_head] = value;
        ++m_tail;
        m_tail %= m_capacity;
    } else {
            if (m_head == 0)
                m_head = m_capacity;
            m_buf[--m_head] = value;
            m_head %= m_capacity;
        }
}

template <typename T>
T Deque<T>::pop_back() {
    if (empty())
        return -1;

    if (m_tail == 0) {
        m_tail = m_capacity;
    }

    --m_tail;
    m_tail %= m_capacity;

    return m_buf[m_tail];
}

template <typename T>
T Deque<T>::pop_front() {
    if (empty())
        return -1;

    int buf_value =  m_buf[m_head];
    m_head = (m_head + 1) % m_capacity;
    return buf_value;
}

/*
template <typename T>
void Deque<T>::print_deque() const {
    std::cout << "head: " << m_head << "; end: " << m_tail <<
              " capacity: " << m_capacity << std::endl;

    for (size_t i = 0; i < m_capacity; ++i) {

        if (i == m_head)
            std::cout << "<";


            if (m_head > m_tail)
                if (i < m_tail || i >= m_head) {
                    std::cout << "|" << m_buf[i];
                } else {
                    std::cout << "|x";
                }
            else
            if (i >= m_tail || i < m_head) {
                std::cout << "|x";
            } else {
                std::cout << "|" << m_buf[i];
            }

        std::cout << "|";

            if (i == m_tail)
                std::cout << ">";
    }

    std::cout << std::endl;
}

*/

int main() {
    Deque<int32_t> my_deq;

    int32_t N, command, value;
    bool flag = true;

    std::cin >> N;

    for (size_t i = 0; i < N && flag; ++i) {
        std::cin >> command >> value;
        switch (command) {
            case 1:
                my_deq.push_front(value);
                break;

            case 2:
                if (my_deq.empty() && value != -1)
                        flag = false;
                else
                    if (value !=  my_deq.pop_front())
                        flag = false;
                break;

            case 3:
                my_deq.push_back(value);
                break;

            case 4:

                if ( my_deq.empty() && value != -1 )
                        flag = false;
                else
                    if (value != my_deq.pop_back())
                        flag = false;
                break;

            default:
                break;
        }
    }

    std::cout << (!flag ? "NO": "YES");
    return 0;
}
