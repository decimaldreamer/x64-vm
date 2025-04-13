#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include <memory>

namespace vm
{
    class memory_protection
    {
    public:
        enum class protection_flags : uint8_t
        {
            READ = 1 << 0,
            WRITE = 1 << 1,
            EXECUTE = 1 << 2,
            GUARD = 1 << 3
        };

        struct memory_region
        {
            uint8_t* base;
            size_t size;
            uint8_t flags;
            bool is_guarded;
        };

        bool add_region(uint8_t* base, size_t size, uint8_t flags);
        bool remove_region(uint8_t* base);
        bool check_access(uint8_t* address, uint8_t required_flags);
        bool is_guarded(uint8_t* address);
        void set_guard_region(uint8_t* base, size_t size);
        void clear_guard_region(uint8_t* base);

    private:
        std::map<uint8_t*, memory_region> m_regions;
        std::vector<memory_region> m_guard_regions;

        bool regions_overlap(const memory_region& a, const memory_region& b);
        bool is_in_region(uint8_t* address, const memory_region& region);
    };
} 