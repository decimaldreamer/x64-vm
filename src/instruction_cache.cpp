#include "instruction_cache.hpp"
#include <algorithm>

namespace vm
{
    instruction_cache& instruction_cache::get_instance()
    {
        static instruction_cache instance;
        return instance;
    }

    instruction_cache::instruction_cache()
        : m_max_size(1024)
        , m_hit_count(0)
        , m_miss_count(0)
    {}

    std::shared_ptr<x86::instruction> instruction_cache::get(uint8_t* address)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_cache.find(address);
        if (it != m_cache.end())
        {
            it->second.last_access = ++m_hit_count;
            it->second.access_count++;
            return it->second.instruction;
        }

        m_miss_count++;
        return nullptr;
    }

    void instruction_cache::put(uint8_t* address, std::shared_ptr<x86::instruction> instruction)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_cache.size() >= m_max_size)
        {
            evict_oldest();
        }

        cache_entry entry;
        entry.instruction = instruction;
        entry.last_access = ++m_hit_count;
        entry.access_count = 1;

        m_cache[address] = entry;
    }

    void instruction_cache::clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_cache.clear();
        m_hit_count = 0;
        m_miss_count = 0;
    }

    void instruction_cache::set_max_size(size_t size)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_max_size = size;
        while (m_cache.size() > m_max_size)
        {
            evict_oldest();
        }
    }

    size_t instruction_cache::get_size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_cache.size();
    }

    size_t instruction_cache::get_hit_count() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_hit_count;
    }

    size_t instruction_cache::get_miss_count() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_miss_count;
    }

    float instruction_cache::get_hit_ratio() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        size_t total = m_hit_count + m_miss_count;
        return total > 0 ? static_cast<float>(m_hit_count) / total : 0.0f;
    }

    void instruction_cache::evict_oldest()
    {
        auto oldest = std::min_element(m_cache.begin(), m_cache.end(),
            [](const auto& a, const auto& b) {
                return a.second.last_access < b.second.last_access;
            });

        if (oldest != m_cache.end())
        {
            m_cache.erase(oldest);
        }
    }
} 