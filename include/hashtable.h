#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "allocator.h"
#include "exceptdef.h"
#include "iterator.h"
#include "util.h"
#include "vector.h"

namespace mystl{

    template <typename T>
    struct hashtable_node{
        hashtable_node* next;
        T value;

        //construct
        hashtable_node() = default;
        hashtable_node(const T& v) : next(nullptr), value(v) {};
        hashtable_node(const hashtable_node& node) : next(node.next), value(node.value) {};
        hashtable_node(hashtable_node&& node) : next(node.next), value(mystl::move(node.value)) {
            node.next = nullptr;
        };

    };

    template <typename T, typename HashFunc, typename KeyOfValue, typename EqualKey>
    class hashtable;

    template <typename T, typename HashFunc, typename KeyOfValue, typename EqualKey>
    struct hashtable_iterator;

    template <typename T, typename HashFunc, typename KeyOfValue, typename EqualKey>
    struct const_hashtable_iterator;

    template <typename T, typename HashFunc, typename KeyOfValue, typename EqualKey>
    struct hashtable_iterator : public mystl::iterator<forward_iterator_tag, T>{
        using hashtable_ = mystl::hashtable<T, HashFunc, KeyOfValue, EqualKey>;
        using iterator = mystl::hashtable_iterator<T, HashFunc, KeyOfValue, EqualKey>;
        using const_iterator = mystl::const_hashtable_iterator<T, HashFunc, KeyOfValue, EqualKey>;
        using node_ptr = hashtable_node<T>*;
        using const_node_ptr = const node_ptr;
        using contain_ptr = hashtable_*;
        using const_contain_ptr = const contain_ptr;
        using size_type = size_t;

        using value_type = T;
        using key_type = decltype(KeyOfValue()(mystl::declval<value_type>()));
        using mapped_type = decltype(HashFunc()(mystl::declval<key_type>()));
        using pointer = value_type*;
        using const_pointer = const pointer;
        using reference = value_type&;
        using const_reference = const reference;

        node_ptr node;
        contain_ptr _hashtable;

        //construct
        hashtable_iterator() = default;
        
        hashtable_iterator(const node_ptr& n, const contain_ptr& ht){
            node = n;
            _hashtable = ht;
        }
        
        hashtable_iterator(const iterator& rhs){
            node = rhs.node;
            _hashtable = rhs._hashtable;
        }

        hashtable_iterator(const const_iterator& rhs){
            node = rhs.node;
            _hashtable = rhs._hashtable;
        }

        //operator
        iterator& operator=(const iterator& rhs){
            if(this != &rhs){
                node = rhs.node;
                _hashtable = rhs._hashtable;
            }
            return *this;
        }

        iterator& operator=(const const_iterator& rhs){
            if(this != &rhs){
                node = rhs.node;
                _hashtable = rhs._node;
            }
            return *this;
        }

        reference operator*() const{
            return node->value;
        }

        pointer operator->() const{
            return &(node->value);
        }

        iterator& operator++(){
            const key_type key = _hashtable->_key_of_value(node->value);  
            node = node->next;
            if(node == nullptr){
                mapped_type index = _hashtable->hash(key);
                while(node == nullptr && ++index < _hashtable->buckets_size){
                    node = _hashtable->buckets[index];
                }
            }
            return *this;
        }

        iterator operator++(int){
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

    };

    template <typename T, typename HashFunc, typename KeyOfValue, typename EqualKey>
    struct const_hashtable_iterator : public mystl::iterator<forward_iterator_tag, T>{
        using hashtable_ = mystl::hashtable<T, HashFunc, KeyOfValue, EqualKey>;
        using iterator = mystl::hashtable_iterator<T, HashFunc, KeyOfValue, EqualKey>;
        using const_iterator = mystl::const_hashtable_iterator<T, HashFunc, KeyOfValue, EqualKey>;
        using node_ptr = const hashtable_node<T>*;
        using const_node_ptr = node_ptr;
        using contain_ptr = const hashtable_*;
        using const_contain_ptr = contain_ptr;
        using size_type = size_t;

        using value_type = T;
        using key_type = decltype(KeyOfValue()(mystl::declval<value_type>()));
        using mapped_type = decltype(HashFunc()(mystl::declval<key_type>()));
        using pointer = const value_type*;
        using const_pointer = pointer;
        using reference = const value_type&;
        using const_reference = reference;

        node_ptr node;
        contain_ptr _hashtable;

        //construct
        const_hashtable_iterator() = default;
        
        const_hashtable_iterator(const node_ptr& n, const contain_ptr& ht){
            node = n;
            _hashtable = ht;
        }
        
        const_hashtable_iterator(const iterator& rhs){
            node = rhs.node;
            _hashtable = rhs._hashtable;
        }

        const_hashtable_iterator(const const_iterator& rhs){
            node = rhs.node;
            _hashtable = rhs._hashtable;
        }

        //operator
        const_iterator& operator=(const iterator& rhs){
            if(this != &rhs){
                node = rhs.node;
                _hashtable = rhs._hashtable;
            }
            return *this;
        }

        const_iterator& operator=(const const_iterator& rhs){
            if(this != &rhs){
                node = rhs.node;
                _hashtable = rhs._node;
            }
            return *this;
        }

        reference operator*() const{
            return node->value;
        }

        pointer operator->() const{
            return &(node->value);
        }

        iterator& operator++(){
            const key_type key = _hashtable->_key_of_value(node->value);  
            node = node->next;
            if(node == nullptr){
                mapped_type index = _hashtable->hash(key);
                while(node == nullptr && ++index < _hashtable->buckets_size){
                    node = _hashtable->buckets[index];
                }
            }
            return *this;
        }

        iterator operator++(int){
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

    };

    template <typename T, typename HashFunc, typename KeyOfValue, typename EqualKey>
    class hashtable{
        public:
        using value_type = T;
        using key_type = decltype(KeyOfValue()(mystl::declval<value_type>()));
        using mapped_type = decltype(HashFunc()(mystl::declval<key_type>()));
        using hasher = HashFunc;
        using key_of_value = KeyOfValue;
        using key_equal = EqualKey;

        using node_type = hashtable_node<value_type>;
        using node_ptr = node_type*;
        using bucket_type = mystl::vector<node_ptr>;

        using iterator = mystl::hashtable_iterator<T, HashFunc, KeyOfValue, EqualKey>;
        using const_iterator = mystl::const_hashtable_iterator<T, HashFunc, KeyOfValue, EqualKey>;

        using data_allocator = mystl::allocator<T>;
        using node_allocator = mystl::allocator<node_type>;

        using pointer = value_type*;
        using const_pointer = const pointer;
        using reference = value_type&;
        using const_reference = const reference;
        using size_type = size_t;

        private:
        bucket_type buckets;
        size_type buckets_size;
        hasher hash;
        key_equal equals;
        key_of_value _key_of_value;

        public:
        //construct


        private:

    };

}

#endif