#ifndef ALLOC_H
#define ALLOC_H

#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <new>

namespace mystl{
    class alloc{
        private:
        static constexpr size_t ALIGN = 8;                              // 8 bytes对其
        static constexpr size_t MAX_BYTES = 128;                        // 最大内存块大小
        static constexpr size_t FREELIST_SIZE = MAX_BYTES / ALIGN;

        union FreeListNode{
            FreeListNode* next;
            char data[1];   // 占位符
        };

        static FreeListNode* free_list[FREELIST_SIZE];
        static char* start_free;    // 当前内存池的起始地址
        static char* end_free;      // 当前内存池的结束地址
        static size_t heap_size;    //总共向系统申请了多少字节

        // 对齐后的大小
        static size_t round_up(size_t bytes){
            return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
        }

        // 根据大小得到freelist的index
        static size_t freelist_index(size_t byte){
            return (byte + ALIGN - 1) / ALIGN - 1;
        }

        static void* refill(size_t size);
        static char* chunk_alloc(size_t size, int& nobjs);

        public:
        static void* allocate(size_t bytes);
        static void deallocate(void* p, size_t bytes);

    };

    alloc::FreeListNode* alloc::free_list[FREELIST_SIZE] = {nullptr};
    char* alloc::start_free = nullptr;
    char* alloc::end_free = nullptr;
    size_t alloc::heap_size = 0;

    void* alloc::allocate(size_t bytes){
        if(bytes > MAX_BYTES)
            return malloc(bytes);
        
        size_t index = freelist_index(bytes);
        FreeListNode*& list = free_list[index];
        if(list){
            FreeListNode* result = list;
            list = result->next;
            return result;
        }

        return refill(round_up(bytes));
    }

    void alloc::deallocate(void* p, size_t bytes){
        if(bytes > MAX_BYTES){
            free(p);
            return;
        }

        size_t index = freelist_index(bytes);
        FreeListNode* node = static_cast<FreeListNode*>(p);
        node->next = free_list[index];
        free_list[index] = node;
    }

    // 向对应的free_list补充若干个大小为size的内存块
    // 返回第一块作为allocate的返回值，剩下的挂入对应的free_list链表备用
    // 调用refill的时候说明freelist中对应大小的块已经没了
    void* alloc::refill(size_t size){
        int nobjs = 20;
        char* chunk = chunk_alloc(size, nobjs);
        if(nobjs == 1)
            return chunk;
        
        size_t index = freelist_index(size);
        FreeListNode*& list = free_list[index];
        // 此时list为nullptr可以直接覆盖
        // 将第二个到第nobjs个块挂到对应的freelist
        list = reinterpret_cast<FreeListNode*>(chunk + size);
        FreeListNode* cur = list;
        for(int i = 2; i < nobjs; ++i){
            cur->next = reinterpret_cast<FreeListNode*>(chunk + size * i);
            cur = cur->next;
        }
        cur->next = nullptr;
        return chunk;
    }

    // 从系统堆区分配一块大内存，然后切成多个小块返回给refill
    char* alloc::chunk_alloc(size_t size, int& nobjs){
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;
        
        if(bytes_left >= total_bytes){
            char* result = start_free;
            start_free += total_bytes;
            return result;
        }
        else if(bytes_left >= size){
            nobjs = bytes_left / size;
            char* result = start_free;
            start_free += size * nobjs;
            return result;
        }
        else{
            size_t bytes_to_get = 2 * total_bytes + round_up(heap_size >> 4);
            if(bytes_left > 0){
                size_t index = freelist_index(bytes_left);
                reinterpret_cast<FreeListNode*>(start_free)->next = free_list[index];
                free_list[index] = reinterpret_cast<FreeListNode*>(start_free);
            }

            start_free = static_cast<char*>(malloc(bytes_to_get));
            if(!start_free){
                // 如果 malloc 失败，从更大的 free_list 借用
                for (size_t i = size; i <= MAX_BYTES; i += ALIGN) {
                    size_t index = freelist_index(i);
                    FreeListNode* list = free_list[index];
                    if (list) {
                        free_list[index] = list->next;
                        start_free = reinterpret_cast<char*>(list);
                        end_free = start_free + i;
                        return chunk_alloc(size, nobjs);
                    }
                }
                end_free = nullptr;
                return nullptr;     // 分配失败，可抛出异常
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
    }
}

#endif