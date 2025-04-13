#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include "instruction.hpp"

namespace vm
{
    class instruction_cache
    {
    public:
        struct cache_entry
        {
            std::shared_ptr<x86::instruction> instruction;
            uint64_t last_access;
            uint64_t access_count;
        };

        static instruction_cache& get_instance();

        std::shared_ptr<x86::instruction> get(uint8_t* address);
        void put(uint8_t* address, std::shared_ptr<x86::instruction> instruction);
        void clear();
        void set_max_size(size_t size);
        size_t get_size() const;
        size_t get_hit_count() const;
        size_t get_miss_count() const;
        float get_hit_ratio() const;

    private:
        instruction_cache();
        ~instruction_cache() = default;

        std::unordered_map<uint8_t*, cache_entry> m_cache;
        size_t m_max_size;
        size_t m_hit_count;
        size_t m_miss_count;
        mutable std::mutex m_mutex;

        void evict_oldest();
    };
} 