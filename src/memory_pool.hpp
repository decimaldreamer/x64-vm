#pragma once

#include <cstdint>
#include <vector>
#include <list>
#include <memory>
#include <mutex>

namespace vm
{
    class memory_pool
    {
    public:
        struct block
        {
            uint8_t* data;
            size_t size;
            bool used;
        };

        memory_pool(size_t initial_size = 1024 * 1024); // 1MB default
        ~memory_pool();

        uint8_t* allocate(size_t size);
        void deallocate(uint8_t* ptr);
        void defragment();
        size_t get_total_size() const;
        size_t get_used_size() const;
        size_t get_free_size() const;

    private:
        std::vector<block> m_blocks;
        std::list<size_t> m_free_blocks;
        size_t m_total_size;
        size_t m_used_size;
        std::mutex m_mutex;

        void grow_pool(size_t size);
        size_t find_free_block(size_t size);
        void merge_adjacent_blocks();
    };
} 