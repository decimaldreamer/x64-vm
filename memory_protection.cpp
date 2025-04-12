#include "memory_protection.hpp"

namespace vm
{
    bool memory_protection::add_region(uint8_t* base, size_t size, uint8_t flags)
    {
        memory_region new_region{base, size, flags, false};

        for (const auto& [_, region] : m_regions)
        {
            if (regions_overlap(new_region, region))
            {
                return false;
            }
        }

        m_regions[base] = new_region;
        return true;
    }

    bool memory_protection::remove_region(uint8_t* base)
    {
        return m_regions.erase(base) > 0;
    }

    bool memory_protection::check_access(uint8_t* address, uint8_t required_flags)
    {
        for (const auto& [_, region] : m_regions)
        {
            if (is_in_region(address, region))
            {
                return (region.flags & required_flags) == required_flags;
            }
        }
        return false;
    }

    bool memory_protection::is_guarded(uint8_t* address)
    {
        for (const auto& region : m_guard_regions)
        {
            if (is_in_region(address, region))
            {
                return true;
            }
        }
        return false;
    }

    void memory_protection::set_guard_region(uint8_t* base, size_t size)
    {
        memory_region guard_region{base, size, 0, true};
        m_guard_regions.push_back(guard_region);
    }

    void memory_protection::clear_guard_region(uint8_t* base)
    {
        m_guard_regions.erase(
            std::remove_if(m_guard_regions.begin(), m_guard_regions.end(),
                [base](const memory_region& region) { return region.base == base; }),
            m_guard_regions.end());
    }

    bool memory_protection::regions_overlap(const memory_region& a, const memory_region& b)
    {
        uint8_t* a_end = a.base + a.size;
        uint8_t* b_end = b.base + b.size;
        return (a.base < b_end) && (b.base < a_end);
    }

    bool memory_protection::is_in_region(uint8_t* address, const memory_region& region)
    {
        return address >= region.base && address < (region.base + region.size);
    }
} 