#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

namespace x64vm::security::rasp {

enum class AccessType {
    READ,
    WRITE,
    EXECUTE
};

class RASPManager {
public:
    static RASPManager& getInstance() {
        static RASPManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    RASPManager(const RASPManager&) = delete;
    RASPManager& operator=(const RASPManager&) = delete;

    bool protect_memory_access(void* addr, size_t size, AccessType type);
    bool protect_system_call(int syscall_num, const std::vector<uint64_t>& args);
    bool protect_file_access(const std::string& path, AccessType type);

    // Modern C++ features
    template<typename T>
    bool protect_object(T* obj, AccessType type) {
        return protect_memory_access(obj, sizeof(T), type);
    }

    template<typename T>
    bool protect_array(T* arr, size_t count, AccessType type) {
        return protect_memory_access(arr, sizeof(T) * count, type);
    }

    // Monitoring and statistics
    size_t getViolationCount() const { return violationCount_; }
    size_t getRelaxationCount() const { return relaxationCount_; }
    bool isMonitoringActive() const { return monitoringActive_; }

private:
    RASPManager();
    ~RASPManager();

    void startMonitoring();
    void stopMonitoring();
    void monitoringThread();
    void updateSecurityRules();
    void logViolation(const std::string& type, const std::string& details);

    struct MemoryRule {
        void* startAddr;
        size_t size;
        AccessType allowedType;
        std::atomic<uint32_t> accessCount;
    };

    struct SystemCallRule {
        int syscallNum;
        std::vector<uint64_t> allowedArgs;
        std::atomic<uint32_t> callCount;
    };

    struct FileRule {
        std::string path;
        AccessType allowedType;
        std::atomic<uint32_t> accessCount;
    };

    std::vector<MemoryRule> memoryRules_;
    std::vector<SystemCallRule> systemCallRules_;
    std::vector<FileRule> fileRules_;

    std::thread monitoringThread_;
    std::atomic<bool> monitoringActive_{false};
    std::atomic<size_t> violationCount_{0};
    std::atomic<size_t> relaxationCount_{0};
    mutable std::mutex rulesMutex_;

    static constexpr size_t MAX_VIOLATIONS = 100;
    static constexpr size_t RELAXATION_THRESHOLD = 50;
};

} // namespace x64vm::security::rasp 