#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <initializer_list>
#include <queue>
#include <set>
#include <string>

#include "../include/allocator.h"
#include "../include/vector.h"
#include "../include/list.h"
#include "../include/deque.h"
#include "../include/rb_tree.h"
#include "../include/set.h"
#include "../include/map.h"
#include "../include/hashtable.h"

template <typename key_type>
struct _hashfunction{};

template <>
struct _hashfunction<int>{
    size_t operator()(const int& key){
        return key;
    }
};

template <typename value_type>
struct _key_of_value{};

template <>
struct _key_of_value<int>{
    int operator()(const int& value){
        return value;
    }
};

template <typename key_type>
struct _equal_key{};

template <>
struct _equal_key<int>{
    bool operator()(const int& lhs, const int& rhs){
        return lhs == rhs;
    }
};


int main(){

    std::cout << mystl::next_prime(101) << std::endl;
    mystl::hashtable<int, _hashfunction<int>, _key_of_value<int>, _equal_key<int>> a(100);
    for (int i = 0; i < 500; i++){
        a.insert_unique(i);
    }
    std::cout << a.size() << std::endl;
    std::cout << *(a.find(37)).first << std::endl;
    std::cout << *(a.find(499)).first << std::endl;
    std::cout << *(a.find(0)).first << std::endl;

    mystl::hashtable<int, _hashfunction<int>, _key_of_value<int>, _equal_key<int>> b(100);
    for(int i = 0; i < 10; i++)
        b.insert_unique(i);

    for(int i : b){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    b.erase_unique(0);
    b.erase_unique(5);
    b.erase_unique(9);

    for(int i : b){
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}