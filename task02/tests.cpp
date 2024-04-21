#include <iostream>
#include <memory>
#include "singly_linked_list.h"

struct mock {
    inline static int throw_after;
    inline static int next_id;
    inline static int alive = 0;

    int id;
    int key = -1;

    mock() : id(next_id++) {
        if (--throw_after < 0) {
            throw 10;
        }
        alive++;
    }

    mock(const mock &other) : mock() {
        key = other.key;
    }

    mock(mock &&) = delete;

    mock &operator=(const mock &) = delete;

    mock &operator=(mock &&) = delete;

    ~mock() {
        alive--;
    }
};

struct mock_allocator {
    inline static int alive = 0;
};

void test_default_initializer() {
    mock::next_id = 0;
    mock::throw_after = 6;
    {
        singly_linked_list<mock, mock_allocator> v(3);
        assert(v[0].id == 1);
        assert(v[1].id == 3);
        assert(v[2].id == 5);
        assert(mock::alive == 3);
    }
    assert(mock::alive == 0);
    assert(mock_allocator::alive == 0);
}

void test_default_initializer_throws_exception() {
    mock::next_id = 0;
    mock::throw_after = 3;
    try {
        singly_linked_list<mock, mock_allocator> v(5);
        assert(false && "Exception not thrown");
    } catch (int) {
    }
    assert(mock::alive == 0);
    assert(mock_allocator::alive == 0);
}

void test_n_copies_of_value() {
    mock::next_id = 0;
    mock::throw_after = 7;
    mock m;
    m.key = 123;
    assert(mock::alive == 1);

    try {
        singly_linked_list<mock, mock_allocator> l(5, m);
        assert(l[0].id == 1);
        assert(l[0].key == 123);
        assert(l[1].id == 2);
        assert(l[1].key == 123);
        assert(l[2].id == 3);
        assert(l[2].key == 123);
        assert(l[3].id == 4);
        assert(l[3].key == 123);
        assert(l[4].id == 5);
        assert(l[4].key == 123);
        assert(mock::alive == 6);
    } catch (const std::exception &e) {
        std::cerr << "Exception caught in test_n_copies_of_value: " << e.what() << "\n";
    }

    assert(mock::alive == 1);
    assert(mock_allocator::alive == 0);
}

void test_n_copies_of_value_throws_exception() {
    mock::next_id = 0;
    mock::throw_after = 4;
    mock m;
    m.key = 123;
    assert(mock::alive == 1);

    mock::next_id = 1;

    try {
        singly_linked_list<mock, mock_allocator> l(5, m);
        assert(false && "Exception not thrown");
    } catch (int) {
        // Expected exception
    }

    assert(mock::alive == 1);
    assert(mock_allocator::alive == 0);
}

void test_push_front_operation() {
    singly_linked_list<mock, mock_allocator> l;
    mock::next_id = 0;
    mock::throw_after = 6;
    l.push_back(mock());
    assert(l[0].id == 1);
    mock::next_id = 1;
    l.push_back(mock());
    assert(l[0].id == 1);
    assert(l[1].id == 2);

    mock::next_id = 2;
    l.push_front(mock());
    assert(l[0].id == 3);
    assert(l[1].id == 1);
    assert(l[2].id == 2);

}

void test_find_operation() {
    singly_linked_list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    assert(l.find(2) == 1);
    assert(l.find(4) == -1);
}

void test_pop_front_operation() {
    singly_linked_list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    l.pop_front();
    assert(l[0] == 2);
    assert(l[1] == 3);
}

void test_push_back_operation() {
    singly_linked_list<int> l;
    l.push_back(1);
    l.push_back(2);

    assert(l[0] == 1);
    assert(l[1] == 2);
}

void test_pop_back_operation() {
    singly_linked_list<int> l;
    l.push_back(1);
    l.push_back(2);

    l.pop_back();
    assert(l[0] == 1);
}

void test_insert_operation() {
    singly_linked_list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    l.insert(1, 2, 5);

    assert(l.size() == 5);
    assert(l[0] == 1);
    assert(l[1] == 5);
    assert(l[2] == 5);
    assert(l[3] == 2);
    assert(l[4] == 3);
}

void test_size_operation() {
    singly_linked_list<int> l;
    assert(l.size() == 0);
    l.push_back(1);
    assert(l.size() == 1);
    l.push_back(2);
    assert(l.size() == 2);
}

void test_getters() {
    singly_linked_list<std::string> l;
    static_assert(std::is_same_v<decltype(l[0]), std::string &>);
    static_assert(std::is_same_v<decltype(std::as_const(l)[0]), const std::string &>);
}

int main() {
    test_find_operation();
    test_pop_front_operation();
    test_push_back_operation();
    test_push_front_operation();
    test_pop_back_operation();
    test_default_initializer();
    test_default_initializer_throws_exception();
    test_n_copies_of_value();
    test_n_copies_of_value_throws_exception();
    test_size_operation();
    test_getters();
    test_insert_operation();

    std::cout << "Good Job!" << "\n";
    return 0;
}
