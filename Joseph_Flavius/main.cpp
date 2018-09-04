// Copyrights 2018 Dima Lipko

/*
 * Считалочка
 *
 * В круг выстроено N человек, пронумерованных числами от 1 до N.
 * Будем исключать каждого k-ого до тех пор, пока не уцелеет только один человек.
 *
 * Например, если N=10, k=3, то сначала умрет 3-й, потом 6-й, затем 9-й, затем 2-й,
 * затем 7-й, потом 1-й, потом 8-й, за ним - 5-й, и потом 10-й. Таким образом, уцелеет 4-й.
 *
 * Необходимо определить номер уцелевшего.
 * N, k ≤ 10000.
 *
 * Пример
 *
 * Ввод	   Вывод
 * 10 3    4
 */

#include <iostream>
#include <cassert>

// Ребенок с его порядковым номером
// И указателями на рядом стоящих детей
struct Child {
    int32_t index;
    Child *next;
    Child *prev;
    Child():index(0), next(nullptr), prev(nullptr) {}
    Child(int32_t number, Child *before, Child *after)
            :index(number), prev(before), next(after) {}
};

// Круг детей для считалочки
class Childs_round {
 public:
    explicit Childs_round(size_t);
    ~ Childs_round(); // Деструктор
    Childs_round(const Childs_round&); // Конструктор копирования
    Childs_round(Childs_round&&) noexcept ; // Конструктор перемещения
    Childs_round& operator=(const Childs_round&); // Оператор присваивания копированием
    Childs_round& operator=(Childs_round&&) noexcept; // Оператор присваивания перемещением

    int32_t kill();

    int32_t head_value() {
        return head -> index;
    }
    void next_head(int32_t k);

    void print() {
        Child *our_head = head;
        while (our_head -> next != head){
            std::cout << our_head -> index;
            our_head = our_head -> next;
        }
        std::cout << our_head -> index;
    }

 private:
    Child *head;
};

Childs_round::Childs_round(const size_t N) {
    head = new Child(1, nullptr, nullptr);
    Child *prev_node = head;
    for (size_t i = 2; i <= N; ++i) {
        auto *node = new Child(i, prev_node, nullptr);
        prev_node -> next = node;
        prev_node = node;
    }
    prev_node -> next = head;
    head -> prev = prev_node;
}

// Деструктор
Childs_round::~Childs_round() {
    if (head != nullptr) {
        while (head->next != head) {
            Child *buf = head;
            head->prev->next = head->next;
            head->next->prev = head->prev;
            head = head->next;
            delete buf;
        }
        delete head;
    }
}

// Конструктор копирования
Childs_round::Childs_round(Childs_round const& other): head(new Child){
    Child *other_head = other.head;
    head -> index = other_head -> index;
    Child *current_head = other_head -> next;
    Child * prev = head;
    while (current_head != other_head) {
        auto new_node = new Child;
        prev -> next = new_node;
        new_node -> prev = prev;
        new_node -> index = current_head -> index;
        prev = new_node;
        current_head = current_head -> next;
    }
    prev -> next = head;
    head -> prev = prev;
}

// Оператор присваивания копированием
Childs_round& Childs_round::operator=(Childs_round const& other) {
    Child *other_head = other.head;
    Child *current_head = other.head;
    Child *our_head = this -> head;
    while (current_head -> next != other_head) {

        if (our_head -> next == this -> head) {
            auto new_node = new Child;
            our_head -> next = new_node;
            this -> head -> prev = new_node;
            new_node -> prev = our_head;
            new_node -> next = this -> head;

        }

        our_head -> index  = current_head -> index;
        our_head  = our_head -> next;
        current_head = current_head -> next;
    }

    return *this;
}

// Конструктор перемещения
Childs_round::Childs_round(Childs_round&& other) noexcept {
    head = other.head;
    other.head = nullptr;
}

// Опреатор присваивания перемещением
Childs_round& Childs_round::operator=(Childs_round && other) noexcept {
    head = other.head;
    other.head = nullptr;
}

// Удаляет текущую голову, переводя её на следующий элемент
int32_t Childs_round::kill() {
    assert(head != nullptr);
    assert(head -> next != nullptr);
    head -> next -> prev  = head -> prev;
    head -> prev -> next = head ->next;
    Child *buf = head;
    head = head -> next;
    delete buf;
}

void Childs_round::next_head(const int32_t k) {
    for (int32_t i = 0; i < k; ++i)
        head = head -> next;
}

int main() {
    size_t N = 0;
    int32_t k = 0;
    std::cin >> N >> k;
    Childs_round game(N);
    for (int32_t i = 0; i < N - 1; ++i) {
        game.next_head(k - 1);
        game.kill();
    }
    std::cout << game.head_value();
    return 0;
}
