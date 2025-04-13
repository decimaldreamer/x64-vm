#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <memory>

namespace vm
{
    class debugger
    {
    public:
        struct breakpoint
        {
            uint8_t* address;
            bool enabled;
            std::function<void()> callback;
        };

        struct watchpoint
        {
            uint8_t* address;
            size_t size;
            bool enabled;
            std::function<void()> callback;
        };

        void add_breakpoint(uint8_t* address, std::function<void()> callback = nullptr);
        void remove_breakpoint(uint8_t* address);
        void enable_breakpoint(uint8_t* address);
        void disable_breakpoint(uint8_t* address);

        void add_watchpoint(uint8_t* address, size_t size, std::function<void()> callback = nullptr);
        void remove_watchpoint(uint8_t* address);
        void enable_watchpoint(uint8_t* address);
        void disable_watchpoint(uint8_t* address);

        void step();
        void continue_execution();
        void pause_execution();

        std::vector<uint8_t*> get_call_stack();
        std::string get_register_dump();
        std::string get_memory_dump(uint8_t* address, size_t size);

        void set_trace_mode(bool enabled);
        bool is_trace_mode_enabled() const;

    private:
        std::map<uint8_t*, breakpoint> m_breakpoints;
        std::map<uint8_t*, watchpoint> m_watchpoints;
        bool m_trace_mode;
        bool m_paused;

        void check_breakpoints(uint8_t* address);
        void check_watchpoints(uint8_t* address, size_t size);
    };
} 