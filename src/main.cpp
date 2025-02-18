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

template <typename T, typename Compare, typename KeyOfValue>
void print_rb_tree(mystl::rb_tree<T, Compare, KeyOfValue>& tree)
{
    using base_ptr = typename mystl::rb_tree<T, Compare, KeyOfValue>::base_ptr;

    // 如果整棵树是空的，直接输出并返回
    // if (tree.root_ptr() == nullptr)
    // {
    //     std::cout << "Tree is empty.\n";
    //     return;
    // }

    // 我们用队列实现层次遍历(BFS)
    std::queue<base_ptr> q;
    q.push(tree.root().node);

    // 由于在你的 rb_tree 中，header->parent 指向 root，
    // 所以 root 节点的 parent 就是 header
    // 因此我们可以通过判断 node->parent == tree.end().node 等方式区别是否是 root
    // 不过这里直接与 tree.root()->parent 比较也可，
    // 但最简明的方式往往是与 tree.header 比较。
    auto header_ptr = tree.end().node;  // 即 tree.header

    while (!q.empty())
    {
        base_ptr node = q.front();
        q.pop();
        if (!node)  // 安全判空
            continue;
        
        // node 可能是 header，本例中只想打印真实节点，所以也可在此过滤
        if (node == header_ptr)
            continue;

        // 1. 打印当前节点的值
        std::cout << "Node value: " << node->as_node()->value;

        // 2. 打印颜色
        //   mystl::red == false, mystl::black == true
        //   这里直接用三元表达式来简化
        std::cout << " | color: "
                  << ((node->color == mystl::black) ? "black" : "red");

        // 3. 父节点
        if (node->parent == nullptr || node->parent == header_ptr)
        {
            std::cout << " | parent: null";
        }
        else
        {
            std::cout << " | parent: " << node->parent->as_node()->value;
        }

        // 4. 左孩子
        if (node->left)
        {
            std::cout << " | left child: " << node->left->as_node()->value;
            q.push(node->left);
        }
        else
        {
            std::cout << " | left child: null";
        }

        // 5. 右孩子
        if (node->right)
        {
            std::cout << " | right child: " << node->right->as_node()->value;
            q.push(node->right);
        }
        else
        {
            std::cout << " | right child: null";
        }

        std::cout << std::endl;
    }
}

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
    for(int i = 0; i < 30; i++)
        a.insert_unique(i);

    print_rb_tree(a);

    std::cout << std::endl;

    a.erase_unique(16);
    a.erase_unique(8);
    a.erase_unique(-1);
    print_rb_tree(a);

    return 0;
}