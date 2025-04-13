#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace x64_vm {
namespace debug {

class DebugManager {
public:
    DebugManager();
    ~DebugManager();

    // Basic debugging
    bool attach_to_process(uint64_t process_id);
    bool detach_from_process();
    bool set_breakpoint(uint64_t address);
    bool remove_breakpoint(uint64_t address);

    // Advanced debugging features
    bool enable_memory_watchpoints();
    bool enable_register_tracking();
    bool enable_instruction_tracing();
    bool enable_call_stack_tracing();
    bool enable_memory_access_tracing();

    // Debug analysis
    std::string analyze_crash_dump();
    std::vector<std::string> get_debug_logs();
    std::string generate_debug_report();

    // Performance profiling
    bool start_profiling();
    bool stop_profiling();
    std::string get_profiling_results();

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

} // namespace debug
} // namespace x64_vm 