#ifndef UNORDERED_MAP_H
#define UNORDERED_MAP_H

#include "allocator.h"
#include "exceptdef.h"
#include "iterator.h"
#include "util.h"
#include "hashtable.h"

namespace mystl{

    template <typename Key, typename Value, typename HashFunc = mystl::_hashfunction<Key>, typename Equalkey = mystl::_equalkey<Key>>
    class unordered_map{
        private:
        struct key_of_value{
            Key operator()(const Value& value) const {
                return value.first;
            }
        };

        using rep_type = mystl::hashtable<mystl::pair<const Key, Value>, HashFunc, key_of_value, Equalkey>;
        rep_type ht_;

        public:
        using data_allocate = typename rep_type::data_allocate;
        using key_type = typename rep_type::key_type;
        using mapped_type = typename rep_type::map_type;
        using value_type = typename rep_type::value_type;
        using hasher = typename rep_type::hasher;
        using key_equal = typename rep_type::key_equal;

        using size_type = typename rep_type::size_type;
        using pointer = typename rep_type::pointer;
        using const_pointer = typename rep_type::const_pointer;
        using reference = typename rep_type::reference;
        using const_reference = typename rep_type::const_reference;

        using iterator = typename rep_type::iterator;
        using const_iterator = typename rep_type::const_iterator;

        public:
        unordered_map() : ht_(100){};

        ~unordered_map() = default;

        //iterator
        iterator begin() noexcept{
            return ht_.begin();
        }

        const_iterator begin() const noexcept{
            return ht_.begin();
        }

        iterator end() noexcept{
            return ht_.end();
        }

        const_iterator end() const noexcept{
            return ht_.end();
        }

        //function
        size_type size() const noexcept{
            return ht_.size();
        }

        size_type empty() const noexcept{
            return ht_.empty();
        }

    };
}

#endif