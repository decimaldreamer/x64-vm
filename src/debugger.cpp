#include "debugger.hpp"
#include "register.hpp"
#include "virtual_memory.hpp"
#include <sstream>
#include <iomanip>

namespace vm
{
    void debugger::add_breakpoint(uint8_t* address, std::function<void()> callback)
    {
        m_breakpoints[address] = {address, true, callback};
    }

    void debugger::remove_breakpoint(uint8_t* address)
    {
        m_breakpoints.erase(address);
    }

    void debugger::enable_breakpoint(uint8_t* address)
    {
        auto it = m_breakpoints.find(address);
        if (it != m_breakpoints.end())
        {
            it->second.enabled = true;
        }
    }

    void debugger::disable_breakpoint(uint8_t* address)
    {
        auto it = m_breakpoints.find(address);
        if (it != m_breakpoints.end())
        {
            it->second.enabled = false;
        }
    }

    void debugger::add_watchpoint(uint8_t* address, size_t size, std::function<void()> callback)
    {
        m_watchpoints[address] = {address, size, true, callback};
    }

    void debugger::remove_watchpoint(uint8_t* address)
    {
        m_watchpoints.erase(address);
    }

    void debugger::enable_watchpoint(uint8_t* address)
    {
        auto it = m_watchpoints.find(address);
        if (it != m_watchpoints.end())
        {
            it->second.enabled = true;
        }
    }

    void debugger::disable_watchpoint(uint8_t* address)
    {
        auto it = m_watchpoints.find(address);
        if (it != m_watchpoints.end())
        {
            it->second.enabled = false;
        }
    }

    void debugger::step()
    {
        m_paused = true;
    }

    void debugger::continue_execution()
    {
        m_paused = false;
    }

    void debugger::pause_execution()
    {
        m_paused = true;
    }

    std::vector<uint8_t*> debugger::get_call_stack()
    {
        std::vector<uint8_t*> stack;
        // Implementation depends on your stack implementation
        return stack;
    }

    std::string debugger::get_register_dump()
    {
        std::stringstream ss;
        // Implementation depends on your register implementation
        return ss.str();
    }

    std::string debugger::get_memory_dump(uint8_t* address, size_t size)
    {
        std::stringstream ss;
        for (size_t i = 0; i < size; i++)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') 
               << static_cast<int>(*(address + i)) << " ";
            if ((i + 1) % 16 == 0)
            {
                ss << "\n";
            }
        }
        return ss.str();
    }

    void debugger::set_trace_mode(bool enabled)
    {
        m_trace_mode = enabled;
    }

    bool debugger::is_trace_mode_enabled() const
    {
        return m_trace_mode;
    }

    void debugger::check_breakpoints(uint8_t* address)
    {
        auto it = m_breakpoints.find(address);
        if (it != m_breakpoints.end() && it->second.enabled)
        {
            m_paused = true;
            if (it->second.callback)
            {
                it->second.callback();
            }
        }
    }

    void debugger::check_watchpoints(uint8_t* address, size_t size)
    {
        for (const auto& [_, watchpoint] : m_watchpoints)
        {
            if (watchpoint.enabled && 
                address >= watchpoint.address && 
                address < watchpoint.address + watchpoint.size)
            {
                m_paused = true;
                if (watchpoint.callback)
                {
                    watchpoint.callback();
                }
            }
        }
    }
} 