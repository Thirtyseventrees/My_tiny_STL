#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <initializer_list>
#include <queue>
#include <set>

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

int main(){


    std::set<int> test;
    mystl::set<int> a;
    for(int i = 0; i < 20; i++)
        a.insert(i);

    for(int i = 0; i < 20; i++)
        std::cout << *(a.find(19 - i)) << " ";

    std::cout << std::endl;
    for(int i = 0; i < 20; i++){
        a.erase(i);
        std::cout << a.size() << " ";
    }
    std::cout << std::endl;

    return 0;
}