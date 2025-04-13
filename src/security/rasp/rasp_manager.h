#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>

namespace x64vm {
namespace security {
namespace rasp {

enum class AccessType {
    READ,
    WRITE,
    EXECUTE
};

class RASPManager {
public:
    RASPManager();
    ~RASPManager();

    void protect_memory_access(void* address, size_t size, AccessType type);
    void protect_system_call(int syscall_number, const std::vector<uint64_t>& args);
    void protect_file_access(const std::string& path, AccessType type);

private:
    void initialize_rules();
    bool check_memory_access(void* address, size_t size, AccessType type);
    bool check_system_call(int syscall_number, const std::vector<uint64_t>& args);
    bool check_file_access(const std::string& path, AccessType type);
    void handle_violation(const std::string& message, ...);
    void monitor();
    void check_suspicious_patterns();
    void update_rules();
    void tighten_rules();
    void relax_rules();
    void log_violation(const std::string& message);
    bool is_critical_violation(const std::string& message);
    void terminate_process();
    void block_operation();
    void check_buffer_overflows();
    void check_format_strings();
    void check_race_conditions();
    void check_memory_leaks();

    std::thread monitoring_thread_;
    bool stop_monitoring_;
    size_t violation_count_;

    static constexpr size_t VIOLATION_THRESHOLD = 10;
    static constexpr size_t RELAX_THRESHOLD = 5;

    std::unordered_map<AccessType, std::vector<std::pair<uintptr_t, uintptr_t>>> memory_rules_;
    std::unordered_map<int, std::vector<std::vector<uint64_t>>> system_call_rules_;
    std::unordered_map<AccessType, std::vector<std::string>> file_access_rules_;
};

} // namespace rasp
} // namespace security
} // namespace x64vm 