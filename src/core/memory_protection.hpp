#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <atomic>

namespace x64vm::core {

class MemoryProtection {
public:
    static MemoryProtection& getInstance() {
        static MemoryProtection instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    MemoryProtection(const MemoryProtection&) = delete;
    MemoryProtection& operator=(const MemoryProtection&) = delete;

    bool protectRegion(void* addr, size_t size, uint32_t flags);
    bool unprotectRegion(void* addr);
    bool isProtected(void* addr) const;
    uint32_t getProtectionFlags(void* addr) const;

    // Modern C++ features
    template<typename T>
    bool protectObject(T* obj) {
        return protectRegion(obj, sizeof(T), PROT_READ | PROT_WRITE);
    }

    template<typename T>
    bool protectArray(T* arr, size_t count) {
        return protectRegion(arr, sizeof(T) * count, PROT_READ | PROT_WRITE);
    }

private:
    MemoryProtection() = default;
    ~MemoryProtection() = default;

    struct ProtectionInfo {
        uint32_t flags;
        size_t size;
        std::atomic<uint32_t> accessCount;
    };

    mutable std::mutex mutex_;
    std::unordered_map<void*, ProtectionInfo> protectedRegions_;

    static constexpr uint32_t PROT_READ = 1 << 0;
    static constexpr uint32_t PROT_WRITE = 1 << 1;
    static constexpr uint32_t PROT_EXEC = 1 << 2;
}; 