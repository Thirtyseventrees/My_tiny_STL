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

int main(){

    mystl::map<int, std::string> a;
    a[0] = "yu";
    a[7] = "ft";
    a[2] = "so";
    a[1] = "zu";

    for(auto i : a)
        std::cout << i.second;
    std::cout << std::endl;

    mystl::map<int, std::string> b;
    b[5] = "ciallo";
    std::cout << b[5] << std::endl;

    return 0;
}