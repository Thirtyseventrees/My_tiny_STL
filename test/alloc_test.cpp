#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <initializer_list>
#include <queue>
#include <set>
#include <string>
#include <chrono>


#include "../include/alloc.h"
#include "../include/allocator.h"
#include "../include/vector.h"
#include "../include/list.h"
#include "../include/deque.h"
#include "../include/rb_tree.h"
#include "../include/set.h"
#include "../include/map.h"
#include "../include/hashtable.h"
#include "../include/unordered_map.h"

int main(){
    void* p1 = mystl::alloc::allocate(32);
    std::cout << "Allocated address: " << p1 << std::endl;
    void* p2 = mystl::alloc::allocate(32);
    std::cout << "Allocated address: " << p2 << std::endl;
    void* p3 = mystl::alloc::allocate(64);
    std::cout << "Allocated address: " << p3 << std::endl;
    mystl::alloc::deallocate(p1, 32);
    mystl::alloc::deallocate(p2, 32);
    mystl::alloc::deallocate(p3, 64);

    std::cout << std::endl;

    void* p4 = mystl::alloc::allocate(32);  // 复用之前的内存
    std::cout << "Allocated address: " << p4 << std::endl;
    void* p5 = mystl::alloc::allocate(32);
    std::cout << "Allocated address: " << p5 << std::endl;
    void* p6 = mystl::alloc::allocate(64);
    std::cout << "Allocated address: " << p6 << std::endl;
    mystl::alloc::deallocate(p4, 32);
    mystl::alloc::deallocate(p5, 32);
    mystl::alloc::deallocate(p6, 64);

    std::cout << std::endl;

    void* p7 = mystl::alloc::allocate(87);
    std::cout << "Allocated address: " << p7 << std::endl;
    mystl::alloc::deallocate(p7, 87);
    void* p8 = mystl::alloc::allocate(83);
    std::cout << "Allocated address: " << p8 << std::endl;
    mystl::alloc::deallocate(p7, 83);

    auto start_1 = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < 100000; ++i){
        void* a1 = mystl::alloc::allocate(4);
        void* a2 = mystl::alloc::allocate(4);
        mystl::alloc::deallocate(a1, 4);
        mystl::alloc::deallocate(a2, 4);
    }

    auto end_1 = std::chrono::high_resolution_clock::now();

    auto start_2 = std::chrono::high_resolution_clock::now();
    
    for(int i = 0; i < 100000; ++i){
        int* i1;
        int* i2;
        i1 = mystl::allocator<int>::allocate();
        i2 = mystl::allocator<int>::allocate();
        mystl::allocator<int>::deallocate(i1);
        mystl::allocator<int>::deallocate(i2);
    }

    auto end_2 = std::chrono::high_resolution_clock::now();

    auto duration_1 = std::chrono::duration_cast<std::chrono::milliseconds>(end_1 - start_1);
    auto duration_2 = std::chrono::duration_cast<std::chrono::milliseconds>(end_2 - start_2);

    std::cout << duration_1.count() << std::endl;
    std::cout << duration_2.count() << std::endl;

    return 0;
}