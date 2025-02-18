#include <iostream>
#include <vector>
#include <type_traits>

#include "../include/allocator.h"
#include "../include/vector.h"

template <typename T>
class MyClass {
public:
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

    // Test allocator
    std::cout << std::endl << "Allocator test" << std::endl;
    std::vector<int, mystl::allocator<int>> v1;
    v1.reserve(5);
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1[3] = 4;
    mystl::construct(&v1[4], v1[3]);

    for (int i = 0; i < 5; i++){
        std::cout << v1[i] << " " << std::endl;
    }

    std::vector<MyClass<int>, mystl::allocator<int>> v2;
    v2.reserve(5);
    v2.push_back(MyClass<int>(1, std::vector<int>(1, 2)));
    
    // Test vector
    std::cout << std::endl << "Vector test" << std::endl;
    mystl::vector<int> mv(3,5);
    for (int i : mv){
        std::cout << i << " " << std::endl;
    }

    mv.push_back(6);
    mv.push_back(7);

    std::cout << std::endl;
    for (int i : mv){
        std::cout << i << " " << std::endl;
    }

    mystl::vector<std::vector<int>> mv2(2, std::vector<int>(2,1));
    mv2.push_back(std::vector<int>(2, 2));
    mv2.push_back(std::vector<int>(2, 3));
    mv2[3].push_back(4);
    
    std::cout << std::endl;
    // std::cout << std::is_destructible<TestClass<int>>::value << std::endl;
    // std::cout << mystl::is_destructible<TestClass<int>>::value << std::endl;
    for(std::vector<int> i : mv2){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    //Copy construct
    mystl::vector<int> mv4(mv);

    for (int i : mv4){
        std::cout << i << " " << std::endl;
    }

    std::cout << std::endl;

    mystl::vector<int> mv5 = mv;

    for (int i : mv4){
        std::cout << i << " " << std::endl;
    }

    std::cout << std::endl;

    mystl::vector<mystl::vector<int>> mv3;
    for (int i = 0; i < 16; i++){
        mv3.push_back(mystl::vector<int>());
        for (int j = 0; j < 10; j++){
            mv3[i].push_back(j + i);
        }
    }

    for(auto i : mv3){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    mv3.pop_back();
    for(auto i : mv3){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    mv3.erase(mv3.end());
    for(auto i : mv3){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    mv3.reserve(9);
    std::cout << mv3.size() << std::endl;
    std::cout << mv3.capacity() << std::endl;
    mv3.insert(mv3.end() - 3, mystl::vector<int>(5, 5));
    mv3.insert(mv3.begin(), mystl::vector<int>(7, 7));
    std::cout << std::endl;
    for(auto i : mv3){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << mv3.size() << std::endl;
    std::cout << mv3.capacity() << std::endl;

    std::cout << std::endl;

    mv3.erase(mv3.begin());
    mv3.erase(mv3.begin() + 3);
    mv3.erase(mv3.end() - 1);
    for(auto i : mv3){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << mv3.size() << std::endl;
    std::cout << mv3.capacity() << std::endl;

    std::cout << std::endl;

    mv3.erase(mv3.begin() + 2, mv3.end() - 1);
    for(auto i : mv3){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << mv3.size() << std::endl;
    std::cout << mv3.capacity() << std::endl;

    std::cout << std::endl;
    mv3.insert(mv3.begin() + 1, 3, mystl::vector<int>(3, 3));
    for(auto i : mv3){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << mv3.size() << std::endl;
    std::cout << mv3.capacity() << std::endl;

    // std::cout << std::endl;
    // mv3.insert(mv3.begin() + 1, 5, mystl::vector<int>(5, 5));
    // for(auto i : mv3){
    //     for(int j : i){
    //         std::cout << j << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << mv3.size() << std::endl;
    // std::cout << mv3.capacity() << std::endl;

    std::cout << std::endl;
    mv3.resize(8, mystl::vector(8, 8));
    for(auto i : mv3){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << mv3.size() << std::endl;
    std::cout << mv3.capacity() << std::endl;

    std::cout << std::endl;
    mv3.resize(6, mystl::vector(8, 8));
    for(auto i : mv3){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << mv3.size() << std::endl;
    std::cout << mv3.capacity() << std::endl;    

    mystl::vector<mystl::vector<int>> mv6(mv3);
    std::cout << (mv3 == mv6) << std::endl;
    mv6.pop_back();
    std::cout << (mv3 == mv6) << std::endl;
    mv3.shrink_to_fit();
    std::cout << mv3.size() << std::endl;
    std::cout << mv3.capacity() << std::endl; 

    std::cout << std::endl;
    mv6.emplace_back(mystl::vector<int>(5,5));
    for(auto i : mv6){
        for(int j : i){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }

    std::cout << mystl::is_mystl_vector<mystl::vector<int>>::value << std::endl;
    std::cout << mystl::is_mystl_vector<int>::value << std::endl;

    return 0;
}