#ifndef RB_TREE_H
#define RB_TREE_H

#include "allocator.h"
#include "exceptdef.h"
#include "iterator.h"
#include "util.h"

namespace mystl {

using node_color = bool;

static constexpr node_color red = false;
static constexpr node_color black = true;

template <typename T>
struct base_rb_tree_node;
template <typename T>
struct rb_tree_node;

template <typename T>
struct base_rb_tree_node {

    using base_ptr = base_rb_tree_node<T> *;
    using node_ptr = rb_tree_node<T> *;

    node_color color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;

    node_ptr as_node() {
        return static_cast<node_ptr>(this);
    }

    static base_ptr minimum(base_ptr x) {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

    static base_ptr maxmimum(base_ptr x) {
        while (x->right != nullptr)
            x = x->right;
        return x;
    }
};

template <typename T>
struct rb_tree_node : public base_rb_tree_node<T> {

    using base_ptr = base_rb_tree_node<T> *;
    using node_ptr = rb_tree_node<T> *;

    T value;

    base_ptr as_base() {
        return static_cast<base_ptr>(this);
    }
};

template <typename T>
struct base_rb_tree_iterator : public mystl::iterator<mystl::bidirectional_iterator_tag, T> {

    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using base_ptr = base_rb_tree_node<T> *;
    using node_ptr = rb_tree_node<T> *;
    using _base_rb_tree_node = base_rb_tree_node<T>;
    using _rb_tree_node = rb_tree_node<T>;

    base_ptr node;

    // Construct
    base_rb_tree_iterator() {};
    base_rb_tree_iterator(base_ptr n) : node(n) {};

    void increment() {
        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr)
                node = node->left;
        } else {
            base_ptr father = node->parent;
            while (father->right == node) {
                node = father;
                father = node->parent;
            }
            // Check the case that find the next of the root node which has no right child.
            if (node->right != father)
                node = father;
        }
    }

    void decrement() {
        // If node is head node
        if (node->parent->parent == node && !(node->color))
            node = node->right;
        // has left child
        else if (node->left != nullptr) {
            node = node->left;
            while (node->right != nullptr)
                node = node->right;
        }
        // no left child and not head node
        else {
            base_ptr father = node->parent;
            while (father->left == node) {
                node = father;
                father = node->parent;
            }
            node = father;
        }
    }

    bool operator==(const base_rb_tree_iterator &rhs) {
        return node == rhs.node;
    }

    bool operator!=(const base_rb_tree_iterator &rhs) {
        return node != rhs.node;
    }
};

template <typename T>
struct rb_tree_iterator : public base_rb_tree_iterator<T> {

    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using base_ptr = base_rb_tree_node<T> *;
    using node_ptr = rb_tree_node<T> *;
    using _base_rb_tree_node = base_rb_tree_node<T>;
    using _rb_tree_node = rb_tree_node<T>;

    using iterator = rb_tree_iterator<T>;
    using self = iterator;

    using base_rb_tree_iterator<T>::node;

    // Construct
    rb_tree_iterator() : base_rb_tree_iterator<T>() {};
    rb_tree_iterator(base_ptr n) : base_rb_tree_iterator<T>(n) {};
    rb_tree_iterator(node_ptr n) : base_rb_tree_iterator<T>(n->as_node()) {};
    rb_tree_iterator(const iterator &rhs) : base_rb_tree_iterator<T>((rhs.node)->as_node()) {};

    reference operator*() const {
        return node->as_node()->value;
    }

    pointer operator->() const {
        return &(operator*());
    }

    self &operator++() {
        this->increment();
        return *this;
    }

    self operator++(int) {
        self temp = *this;
        this->increment();
        return temp;
    }

    self &operator--() {
        this->decrement();
        return *this;
    }

    self operator--(int) {
        self temp = *this;
        this->decrement();
        return temp;
    }
};

template <typename T, typename comp, typename KeyOfValue, typename alloc = mystl::allocator<T>>
struct rb_tree {

  public:
    using base_ptr = base_rb_tree_node<T> *;
    using node_ptr = rb_tree_node<T> *;
    using _base_rb_tree_node = base_rb_tree_node<T>;
    using _rb_tree_node = rb_tree_node<T>;

    using allocator_type = alloc;
    using data_allocator = alloc;
    using base_node_allocator = mystl::allocator<_base_rb_tree_node>;
    using node_allocator = mystl::allocator<_rb_tree_node>;

    using value_type = T;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using key_compare = comp;
    using key_of_value = KeyOfValue;
    using size_type = size_t;
    using iterator = rb_tree_iterator<T>;

  protected:
    node_ptr create_node(const value_type &value) {
        node_ptr p = node_allocator::allocate();
        p->color = red;
        p->left = nullptr;
        p->right = nullptr;
        data_allocator::construct(&(p->value), value);
        return p;
    }

    template <typename... Args>
    node_ptr create_node(Args &&...args) {
        node_ptr p = node_allocator::allocate();
        p->color = red;
        p->left = nullptr;
        p->right = nullptr;
        data_allocator::construct(&(p->value), mystl::forward<Args>(args)...);
        return p;
    }

    // copy value and color
    node_ptr copy_node(const node_ptr &rhs) {
        node_ptr p = create_node(rhs->value);
        p->color = rhs->color;
        p->left = nullptr;
        p->right = nullptr;
        return p;
    }

    void destroy_node(node_ptr p) {
        data_allocator::destroy(&(p->value));
        node_allocator::deallocate(p);
    }

  private:
    size_type node_count;
    base_ptr header;
    key_compare _key_comp;
    key_of_value _key_of_value;

  public:
    // Construct
    rb_tree(const key_compare &c = comp(), const key_of_value &k = KeyOfValue()) : _key_comp(c), _key_of_value(k) { rb_tree_init(); }

    ~rb_tree() {
        clear();
        base_node_allocator::deallocate(header);
    }

    // iterator
    iterator begin() {
        return iterator(leftmost_ptr());
    }

    iterator end() {
        return iterator(header);
    }

    iterator root() {
        return iterator(header->parent);
    }

    iterator leftmost() {
        return iterator(leftmost_ptr());
    }

    iterator rightmost() {
        return iterator(rightmost_ptr());
    }

    iterator find(const value_type &value) {
        return iterator(find_ptr(value));
    }

    iterator insert_equal(const value_type &value) {
        return iterator(insert_node(value));
    }

    mystl::pair<iterator, bool> insert_unique(const value_type &value) {
        return insert_unique_node(value);
    }

    bool erase_unique(const value_type& value){
        return erase_node(value);
    }

    /***************/
    // private function
  private:
    base_ptr &root_ptr() const {
        return header->parent;
    }

    base_ptr &leftmost_ptr() const {
        return header->left;
    }

    base_ptr &rightmost_ptr() const {
        return header->right;
    }

    void rb_tree_init() {
        header = base_node_allocator::allocate();
        header->color = red;
        header->parent = nullptr;
        header->left = header;
        header->right = header;
        node_count = 0;
    }

    base_ptr find_ptr(const value_type &value) {
        node_ptr cur = header->parent->as_node();
        auto key = _key_of_value(value);
        while (cur != nullptr) {
            auto cur_key = _key_of_value(cur->value);
            if (_key_comp(key, cur_key)) {
                cur = cur->left->as_node();
            } else {
                if (!_key_comp(cur_key, key))
                    return cur;
                cur = cur->right->as_node();
            }
        }
        return header;
    }

    base_ptr next_ptr(base_ptr node) {
        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr)
                node = node->left;
        } else {
            base_ptr father = node->parent;
            while (father->right == node) {
                node = father;
                father = node->parent;
            }
            // Check the case that find the next of the root node which has no right child.
            if (node->right != father)
                node = father;
        }
        return node;
    }

    base_ptr insert_node(const value_type &value) {
        node_ptr n = create_node(value);
        insert_node_at(get_parent_pos_of_value(value), n);
        return n;
    }

    bool erase_node(const value_type &value) {
        base_ptr node = find_ptr(value);
        if (node == header)
            return false;
        exchange_node(node, next_ptr(node));
        node_color original_color = node->color;
        base_ptr father = node->parent;
        base_ptr child = node->right;
        if (child != nullptr)
            child->parent = father;
        if (father->left == node)
            father->left = child;
        else
            father->right = child;
        node->right = nullptr;
        node->parent = nullptr;
        destroy_node(node->as_node());
        if (original_color == black)
            rb_tree_erase_reblance(child, father);
        --node_count;
        return true;
    }

    mystl::pair<iterator, bool> insert_unique_node(const value_type &value) {
        node_ptr father = get_parent_pos_of_value(value)->as_node();
        if (!_key_comp(_key_of_value(value), _key_of_value(father->value)) && !_key_comp(_key_of_value(father->value), _key_of_value(value)))
            return mystl::pair<iterator, bool>(end(), false);
        node_ptr n = create_node(value);
        insert_node_at(father, n);
        return mystl::pair<iterator, bool>(iterator(father), true);
    }

    base_ptr get_parent_pos_of_value(const value_type &value) {
        auto key = _key_of_value(value);
        base_ptr x = root_ptr();
        base_ptr father = header;
        while (x != nullptr) {
            father = x;
            x = _key_comp(key, _key_of_value(x->as_node()->value)) ? x->left : x->right;
        }
        return father;
    }

    /**
     *  @brief  Inserts given node to the child of father
     *  @param  father  The parent of node after insert.
     *  @param  node  Node to be inserted.
     *  @return  pointer of node.
     * */
    base_ptr insert_node_at(base_ptr father, node_ptr node) {
        node->parent = father;
        base_ptr base_node = node->as_base();
        // If node is root
        if (father == header) {
            father->parent = base_node;
            father->left = base_node;
            father->right = base_node;
        }
        // node is the left child of father
        else if (_key_comp(_key_of_value(node->value), _key_of_value(father->as_node()->value))) {
            father->left = base_node;
            if (header->left == father)
                header->left = base_node;
        }
        // node is right child of father
        else {
            father->right = base_node;
            if (header->right == father)
                header->right = base_node;
        }
        rb_tree_insert_reblance(base_node);
        ++node_count;
        return base_node;
    }

    /*-------------------------------------------------*\
    |    r <- b -> r    |    r <- b      |   b -> r     |
    |   / \       / \       / \                  / \    |
    |  1   2     3   4     5   6                7   8   |
    \*-------------------------------------------------*/
    void rb_tree_insert_reblance(base_ptr node) {
        // node is not root and it's parent color is red(false)
        while (node != header->parent && !(node->parent->color)) {
            // case 5 and 6
            if (node->parent->parent->right == nullptr || node->parent->parent->right->color == black) {
                // case 5(LL)
                if (node->parent->left == node) {
                    right_rotate(node->parent->parent);
                    node->parent->color = black;
                    node->parent->right->color = red;
                }
                // case 6(LR)
                else {
                    node->color = black;
                    node->parent->parent->color = red;
                    left_rotate(node->parent);
                    right_rotate(node->parent);
                }
            }
            // case 7 and 8
            else if (node->parent->parent->left == nullptr || node->parent->parent->left->color == black) {
                // case 8(RR)
                if (node->parent->right == node) {
                    left_rotate(node->parent->parent);
                    node->parent->color = black;
                    node->parent->left->color = red;
                }
                // case 7(RL)
                else {
                    node->color = black;
                    node->parent->parent->color = red;
                    right_rotate(node->parent);
                    left_rotate(node->parent);
                }
            }
            // case 1 and 2
            else if (node->parent->parent->left == node->parent) {
                node->parent->color = black;
                node->parent->parent->right->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }
            // case 3 and 4
            else {
                node->parent->color = black;
                node->parent->parent->left->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }
        }
        header->parent->color = black;
    }

    void rb_tree_erase_reblance(base_ptr node, base_ptr father) {
        while (node != header->parent && (node == nullptr || node->color == black)) {
            base_ptr brother;
            // left child
            if (node == father->left) {
                brother = father->right;
                // the color of brother is red
                if (brother->color == red) {
                    brother->color = black;
                    father->color = red;
                    left_rotate(father);
                }
                node_color father_color = father->color;
                brother = father->right;
                // if the color of brother is black
                if (brother->color == black) {
                    // if brother node has child
                    if (brother->right != nullptr) {
                        left_rotate(brother->parent);
                        brother->color = father_color;
                        brother->left->color = black;
                        brother->right->color = black;
                        // finish, exit
                        node = header->parent;
                    } else if (brother->left != nullptr) {
                        right_rotate(brother);
                        left_rotate(father);
                        father->color = black;
                        brother->color = black;
                        brother->parent->color = father_color;
                        // finish, exit
                        node = header->parent;
                    }
                    // brother has no child
                    else {
                        // the color of father is red
                        if (father_color == red) {
                            brother->color = red;
                            father_color = black;
                        }
                        // the color of father is black
                        else {
                            brother->color = red;
                            node = father;
                            father = father->parent;
                        }
                    }
                }
            }
            // right child
            else {
                brother = father->left;
                // the color of brother is red
                if (brother->color == red) {
                    brother->color = black;
                    father->color = red;
                    right_rotate(father);
                }
                node_color father_color = father->color;
                brother = father->left;
                // if the color of brother is black
                if (brother->color == black) {
                    // if brother node has child
                    if (brother->left != nullptr) {
                        right_rotate(father);
                        brother->color = father_color;
                        brother->left->color = black;
                        brother->right->color = black;
                        // finish, exit
                        node = header->parent;
                    } else {
                        left_rotate(brother);
                        right_rotate(father);
                        father->color = black;
                        brother->color = black;
                        brother->parent->color = father_color;
                        // finish, exit
                        node = header->parent;
                    }
                }
                // brother has no child
                else {
                    // the color of father is red
                    if (father_color == red) {
                        brother->color = red;
                        father_color = black;
                    }
                    // the color of father is black
                    else{
                        brother->color = red;
                        node = father;
                        father = father->parent;
                    }
                }
            }
        }
        if (node != nullptr)
            node->color = black;
    }

    /*---------------------------------------*\
    |       p                         p       |
    |      / \                       / \      |
    |     x   d    rotate left      y   d     |
    |    / \       ===========>    / \        |
    |   a   y                     x   c       |
    |      / \                   / \          |
    |     b   c                 a   b         |
    \*---------------------------------------*/
    void left_rotate(base_ptr x) {
        base_ptr y = x->right;
        // deal with node b
        x->right = y->left;
        if (y->left != nullptr)
            y->left->parent = x;
        // deal with node p
        y->parent = x->parent;
        // If x is root
        if (x == header->parent) {
            header->parent = y;
        }
        // x is right child
        else if (x->parent->right == x) {
            x->parent->right = y;
        }
        // x is left child
        else {
            x->parent->left = y;
        }
        // deal with x and y
        y->left = x;
        x->parent = y;
    }

    /*----------------------------------------*\
    |     p                         p          |
    |    / \                       / \         |
    |   d   x      rotate right   d   y        |
    |      / \     ===========>      / \       |
    |     y   a                     b   x      |
    |    / \                           / \     |
    |   b   c                         c   a    |
    \*----------------------------------------*/
    void right_rotate(base_ptr x) {
        base_ptr y = x->left;
        // deal with node c
        x->left = y->right;
        if (y->right != nullptr)
            y->right->parent = x;
        // deal with node p
        y->parent = x->parent;
        // If x is root
        if (x == header->parent) {
            header->parent = y;
        }
        // x is right child
        else if (x->parent->right == x) {
            x->parent->right = y;
        }
        // x is left child
        else {
            x->parent->left = y;
        }
        // deal with x and y
        y->right = x;
        x->parent = y;
    }

    void exchange_node(base_ptr x, base_ptr y) {
        if (x->left != nullptr)
            x->left->parent = y;
        if (x->right != nullptr)
            x->right->parent = y;
        if (y->left != nullptr)
            y->left->parent = x;
        if (y->right != nullptr)
            y->right->parent = x;
        mystl::swap(x->left, y->left);
        mystl::swap(x->right, y->right);
        // If x is root
        if (x == header->parent) {
            // if y is left child
            if (y->parent->left == y)
                y->parent->left = x;
            // y is right child
            else
                y->parent->right = x;
            x->parent->parent = y;
            mystl::swap(x->parent, y->parent);
        }
        // if y is root
        else if (y == header->parent) {
            // if x is left child
            if (x->parent->left == x)
                x->parent->left = y;
            // x is right child
            else
                x->parent->right = y;
            y->parent->parent = x;
            mystl::swap(x->parent, y->parent);
        } else {
            // if y is left child
            if (y->parent->left == y)
                y->parent->left = x;
            // y is right child
            else
                y->parent->right = x;
            // if x is left child
            if (x->parent->left == x)
                x->parent->left = y;
            // x is right child
            else
                x->parent->right = y;
            mystl::swap(x->parent, y->parent);
        }
        mystl::swap(x->color, y->color);
    }

    void erase_sub_tree(base_ptr r) {
        while (r != nullptr) {
            erase_sub_tree(r->right);
            base_ptr l = r->left;
            destroy_node(r->as_node());
            --node_count;
            r = l;
        }
    }

    void clear() {
        if (node_count != 0) {
            erase_sub_tree(root_ptr());
            header->left = header;
            header->right = header;
            header->parent = nullptr;
            node_count = 0;
        }
    }
};

}

#endif