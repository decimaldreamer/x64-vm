#include "memory_pool.hpp"
#include <algorithm>
#include <cstring>

namespace vm
{
    memory_pool::memory_pool(size_t initial_size)
        : m_total_size(initial_size)
        , m_used_size(0)
    {
        m_blocks.push_back({new uint8_t[initial_size], initial_size, false});
        m_free_blocks.push_back(0);
    }

    memory_pool::~memory_pool()
    {
        for (const auto& block : m_blocks)
        {
            delete[] block.data;
        }
    }

    uint8_t* memory_pool::allocate(size_t size)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        size_t block_index = find_free_block(size);
        if (block_index == static_cast<size_t>(-1))
        {
            grow_pool(size);
            block_index = find_free_block(size);
        }

        auto& block = m_blocks[block_index];
        block.used = true;
        m_used_size += size;

        if (block.size > size)
        {
            block new_block{block.data + size, block.size - size, false};
            block.size = size;
            m_blocks.insert(m_blocks.begin() + block_index + 1, new_block);
            m_free_blocks.push_back(block_index + 1);
        }

        m_free_blocks.remove(block_index);
        return block.data;
    }

    void memory_pool::deallocate(uint8_t* ptr)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = std::find_if(m_blocks.begin(), m_blocks.end(),
            [ptr](const block& b) { return b.data == ptr; });

        if (it != m_blocks.end())
        {
            it->used = false;
            m_used_size -= it->size;
            m_free_blocks.push_back(std::distance(m_blocks.begin(), it));
            merge_adjacent_blocks();
        }
    }

    void memory_pool::defragment()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        std::vector<block> new_blocks;
        new_blocks.reserve(m_blocks.size());

        for (const auto& block : m_blocks)
        {
            if (block.used)
            {
                new_blocks.push_back(block);
            }
        }

        size_t free_size = m_total_size - m_used_size;
        if (free_size > 0)
        {
            new_blocks.push_back({new uint8_t[free_size], free_size, false});
        }

        m_blocks = std::move(new_blocks);
        m_free_blocks.clear();
        m_free_blocks.push_back(m_blocks.size() - 1);
    }

    size_t memory_pool::get_total_size() const
    {
        return m_total_size;
    }

    size_t memory_pool::get_used_size() const
    {
        return m_used_size;
    }

    size_t memory_pool::get_free_size() const
    {
        return m_total_size - m_used_size;
    }

    void memory_pool::grow_pool(size_t size)
    {
        size_t new_size = std::max(m_total_size * 2, m_total_size + size);
        m_blocks.push_back({new uint8_t[new_size - m_total_size], new_size - m_total_size, false});
        m_free_blocks.push_back(m_blocks.size() - 1);
        m_total_size = new_size;
    }

    size_t memory_pool::find_free_block(size_t size)
    {
        for (const auto& index : m_free_blocks)
        {
            if (m_blocks[index].size >= size)
            {
                return index;
            }
        }
        return static_cast<size_t>(-1);
    }

    void memory_pool::merge_adjacent_blocks()
    {
        for (size_t i = 0; i < m_blocks.size() - 1; ++i)
        {
            if (!m_blocks[i].used && !m_blocks[i + 1].used)
            {
                m_blocks[i].size += m_blocks[i + 1].size;
                m_blocks.erase(m_blocks.begin() + i + 1);
                --i;
            }
        }
    }
} 