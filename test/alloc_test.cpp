#include <iostream>
#include <vector>
#include <list>
#include <type_traits>
#include <initializer_list>
#include <queue>
#include <set>
#include <string>

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

    return 0;
}