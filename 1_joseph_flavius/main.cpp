#include <iostream>
#include "cassert"


int winner (int N, int offset) {
    int person_index  = 0;

    for (int i = 0; i < N; ++i) {
        person_index = (person_index + offset) % (i + 1);
        std::cout<< person_index << std::endl;
    }

    return ++person_index;
}

int main() {
    int N = 0, offset = 0;
    std::cin >> N >> offset;
    assert( N > 0 && offset > 0 );
    std::cout << winner(N,offset);
    return 0;
}