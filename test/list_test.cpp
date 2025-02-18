#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <initializer_list>

#include "../include/allocator.h"
#include "../include/vector.h"
#include "../include/list.h"

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
    std::list<int> test;
    mystl::list<int> a;
    std::cout << (a.end()++ == a.begin()) << std::endl;
    //std::cout << a.size() << std::endl;
    for(int i = 0; i < 10; i++){
        a.push_back(i);
    }
    
    for (int i = 1; i < 10; i++){
        a.push_front(i);
    }

    a.erase(a.cbegin());
    a.erase(a.cbegin() + 4);
    a.erase(a.cend() - 1);
    a.insert(a.begin() + 4, 4);
    a.pop_front();
    a.pop_back();

    for(auto i : a){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 5; i++){
        a.insert(a.begin() + 1, 37);
    }

    for(auto i : a){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    a.remove(37);
    for(auto i : a){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i < 5; i++){
        a.insert(a.begin() + 1, 39);
    }
    a.unique();
    for(auto i : a){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    //check transfer
    std::cout << std::endl;
    mystl::list<int> a2;
    for(int i = 0; i < 5; i++){
        a2.push_back(i + 50);
    }
    std::cout << "a2: ";
    for(auto i : a2){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    //transfer a part of a2 into a
    a.transfer(a.end(), a2.begin() + 1, a2.end() - 2);
    std::cout << "a: ";
    for(auto i : a){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "a2: ";
    for(auto i : a2){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    a.transfer(a2.begin() + 1, a.begin() + 3, a.begin() + 5);
    for(auto i : a){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    for(auto i : a2){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    a2.transfer(a2.begin(), a2.begin() + 1, a2.begin() + 3);
    for(auto i : a2){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl;
    a.transfer(a.end(), a.begin(), a.begin() + 3);
    for(auto i : a){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << mystl::is_the_same_type<typename mystl::iterator_traits<mystl::list_iterator<int>>::value_type, mystl::list_iterator<int>::value_type>::value << std::endl;
    mystl::list<int> a3(a.begin(), a.end());

    for(auto i : a3){
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << *(a3.find(8)) << std::endl;

    std::cout << std::endl;
    mystl::vector<int> v(5, 5);
    mystl::list<int> a4(v.begin(), v.end());
    for(auto i : a4){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}