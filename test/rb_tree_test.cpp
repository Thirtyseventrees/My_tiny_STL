#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <initializer_list>
#include <queue>

#include "../include/allocator.h"
#include "../include/vector.h"
#include "../include/list.h"
#include "../include/deque.h"
#include "../include/rb_tree.h"
#include "../include/set.h"

template <typename T>
class MyClass {
public:
    MyClass() = delete;
    MyClass(int x, std::vector<T>&& v) : index(x), vec(v) {};
    std::vector<T> getVec() const {
        return vec;
    }

    int getIndex() const {
        return index;
    }
private:
    int index;
    std::vector<T> vec;
};

template <typename T>
class TestClass{
    public:
    ~TestClass() = delete;
};

struct key_of_value{
    template <typename T>
    T operator()(const T& value) const{
        return value;
    }
};

struct compare_less{
    template <typename T>
    bool operator()(const T& lhs, const T& rhs) const{
        return lhs < rhs;
    }
};

int main(){

    mystl::rb_tree<int, compare_less, key_of_value> a;
    for(int i = 1; i < 30; i++)
        a.insert_unique(i);

    print_rb_tree(a);

    std::cout << std::endl;

    for(int i = 1; i < 15; i++){
        a.erase_unique(i);
        std::cout << "Delete: " << i << std::endl;
        print_rb_tree(a);
        std::cout << "leftmost: " << *(a.leftmost()) << " rightmost: " << *(a.rightmost()) << std::endl;
        std::cout <<std::endl;
    }

    for(int i = 29; i > 15; i--){
        a.erase_unique(i);
        std::cout << "Delete: " << i << std::endl;
        print_rb_tree(a);
        std::cout << "leftmost: " << *(a.leftmost()) << " rightmost: " << *(a.rightmost()) << std::endl;
        std::cout <<std::endl;
    }

    a.erase_unique(15);
    std::cout << "Delete: 15" << std::endl;
    print_rb_tree(a);

    std::cout << std::endl;
    mystl::rb_tree<int, compare_less, key_of_value> b;
    for(int i = 0; i < 5; i++)
        b.insert_equal(i);
    for(int i = 0; i < 5; i++)
        b.insert_equal(3);
    print_rb_tree(b);

    std::cout << std::endl;
    std::cout << b.count_multi(3) << std::endl;
    std::cout << b.erase_multi(3) << std::endl;
    std::cout << b.erase_multi(-1) << std::endl;
    std::cout << b.count_multi(-1) << std::endl;
    std::cout << b.count_multi(0) << std::endl;
    print_rb_tree(b);

    return 0;
}