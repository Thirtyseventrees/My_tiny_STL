#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <initializer_list>

#include "../include/allocator.h"
#include "../include/vector.h"
#include "../include/list.h"
#include "../include/deque.h"

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

    mystl::deque<mystl::vector<int>> a(10, mystl::vector<int>(10, 5));

    for (auto i : a){
        for(auto j : i)
            std::cout << j << " ";
        std::cout << std::endl;
    }
    
    for(int i = 0; i < static_cast<int>(a.size()); i++){
        for(int j = 0; j < static_cast<int>(a[i].size()); j++)
            a[i][j] = i + j;
    }

    std::cout << std::endl;

    for (auto i : a){
        for(auto j : i)
            std::cout << j << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // std::cout << std::endl;
    // mystl::deque<int> b(1023, 1);
    // std::cout << b.size() << std::endl;
    // std::cout << b.capacity() << std::endl; 
    // std::cout << b.front() << std::endl;


    // b.push_front(2);  
    // std::cout << b.size() << std::endl;
    // std::cout << b.capacity() << std::endl; 
    // std::cout << b.front() << std::endl;
    // b.push_front(3); 
    // std::cout << b.size() << std::endl;
    // std::cout << b.capacity() << std::endl; 
    // std::cout << b.front() << std::endl;
    // for (int i = 0; i < 1024; i++){
    //     b.push_back(4);
    // }
    // std::cout << b.size() << std::endl;
    // std::cout << b.capacity() << std::endl; 
    // std::cout << b.back() << std::endl;

    // std::cout << std::endl;

    // mystl::deque<int> c;

    // for (int i = 0; i < 1024; i++){
    //     c.push_back(i);
    // }
    // std::cout << c.size() << std::endl;
    // std::cout << c.capacity() << std::endl; 
    // std::cout << c.front() << " " << c.back() << std::endl;
    // c.pop_back();
    // c.pop_front();
    // std::cout << c.size() << std::endl;
    // std::cout << c.capacity() << std::endl; 
    // std::cout << c.front() << " " << c.back() << std::endl;  
    // c.push_front(0);
    // std::cout << c.size() << std::endl;
    // std::cout << c.capacity() << std::endl; 
    // std::cout << c.front() << " " << c.back() << std::endl; 
    // c.pop_front();
    // std::cout << c.size() << std::endl;
    // std::cout << c.capacity() << std::endl; 
    // std::cout << c.front() << " " << c.back() << std::endl; 
    // for(int i = 0; i < 256; i++){
    //     c.pop_back();
    //     c.pop_front();
    // }
    // std::cout << c.size() << std::endl;
    // std::cout << c.capacity() << std::endl; 
    // std::cout << c.front() << " " << c.back() << std::endl; 
    // std::cout << std::endl;

    // std::cout << *(c.find(c.begin(), c.end(), 512)) << std::endl;

    mystl::deque<mystl::vector<int>> d(a.begin(), a.end());

    for (auto i : d){
        for(auto j : i)
            std::cout << j << " ";
        std::cout << std::endl;
    }

    std::cout << std::endl;
    d.insert(d.begin() + 3, d.back());
    d.insert(d.begin() + 1, d.back());
    d.insert(d.begin() + 4, d.back());
    d.insert(d.end() - 2, d.front());
    d.insert(d.end() - 3, d.front());

    for (auto i : d){
        for(auto j : i)
            std::cout << j << " ";
        std::cout << std::endl;
    }

    std::cout << std::endl;
    d.erase(d.begin() + 4);
    d.erase(d.end() - 3);
    for (auto i : d){
        for(auto j : i)
            std::cout << j << " ";
        std::cout << std::endl;
    }

    return 0;
}