#include "nested_virtualization.hpp"
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <thread>
#include <fstream>

namespace vm
{
    nested_virtualization& nested_virtualization::get_instance()
    {
        static nested_virtualization instance;
        return instance;
    }

    void nested_virtualization::create_nested_vm(const std::string& name, std::shared_ptr<virtual_machine> parent_vm)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_nested_vms.find(name) != m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " already exists");
        }

        vm_state state;
        state.vm = std::make_shared<virtual_machine>();
        state.memory.resize(1024 * 1024); // 1MB memory for nested VM
        state.is_paused = false;
        state.last_exit_reason = 0;

        // Setup EPT and VMCS with optimizations
        setup_ept(reinterpret_cast<uint8_t*>(state.memory.data()), 
                 parent_vm->get_memory().data());
        setup_vmcs(state.vm, parent_vm);

        // Initialize performance optimizations
        optimize_memory_access();
        prefetch_instructions();
        cache_translations();

        m_nested_vms[name] = std::move(state);
    }

    void nested_virtualization::pause_nested_vm(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        it->second.is_paused = true;
        save_nested_vm_state(name);
    }

    void nested_virtualization::resume_nested_vm(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        it->second.is_paused = false;
        restore_nested_vm_state(name);
    }

    void nested_virtualization::save_nested_vm_state(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }

        // Save current VM state
        it->second.saved_state = it->second.memory;
        log_vm_state(name);
    }

    void nested_virtualization::restore_nested_vm_state(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }

        // Restore saved state
        it->second.memory = it->second.saved_state;
        validate_vm_state(name);
    }

    void nested_virtualization::register_exit_handler(const std::string& name, std::function<void()> handler)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        it->second.exit_handlers.push_back(handler);
    }

    void nested_virtualization::register_entry_handler(const std::string& name, std::function<void()> handler)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        it->second.entry_handlers.push_back(handler);
    }

    void nested_virtualization::handle_vmexit(uint8_t* guest_rip, uint8_t* host_rip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        // Execute all exit handlers
        for (const auto& pair : m_nested_vms)
        {
            for (const auto& handler : pair.second.exit_handlers)
            {
                handler();
            }
        }

        // Log exit reason and state
        for (auto& pair : m_nested_vms)
        {
            pair.second.last_exit_reason = reinterpret_cast<uint64_t>(guest_rip);
            log_vm_state(pair.first);
        }
    }

    void nested_virtualization::handle_vmentry(uint8_t* guest_rip, uint8_t* host_rip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        // Execute all entry handlers
        for (const auto& pair : m_nested_vms)
        {
            for (const auto& handler : pair.second.entry_handlers)
            {
                handler();
            }
        }

        // Validate state before entry
        for (const auto& pair : m_nested_vms)
        {
            validate_vm_state(pair.first);
        }
    }

    bool nested_virtualization::is_vm_paused(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        return it->second.is_paused;
    }

    uint64_t nested_virtualization::get_last_exit_reason(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        return it->second.last_exit_reason;
    }

    uint64_t nested_virtualization::get_vm_instruction_count(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        return it->second.vm->get_instruction_count();
    }

    uint64_t nested_virtualization::get_vm_memory_usage(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        return it->second.memory.size();
    }

    double nested_virtualization::get_vm_cpu_usage(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        return it->second.vm->get_cpu_usage();
    }

    void nested_virtualization::optimize_memory_access()
    {
        // Implement memory access optimizations
        // This could include:
        // - Memory access pattern analysis
        // - Cache line optimization
        // - Memory prefetching
    }

    void nested_virtualization::prefetch_instructions()
    {
        // Implement instruction prefetching
        // This could include:
        // - Branch prediction
        // - Instruction cache optimization
        // - Pipeline optimization
    }

    void nested_virtualization::cache_translations()
    {
        // Implement translation caching
        // This could include:
        // - TLB optimization
        // - Page table caching
        // - Address translation caching
    }

    void nested_virtualization::log_vm_state(const std::string& name)
    {
        auto it = m_nested_vms.find(name);
        if (it != m_nested_vms.end())
        {
            // Log VM state to file or console
            std::ofstream log("vm_state_" + name + ".log");
            log << "VM State for " << name << "\n";
            log << "Memory size: " << it->second.memory.size() << "\n";
            log << "Is paused: " << (it->second.is_paused ? "true" : "false") << "\n";
            log << "Last exit reason: " << it->second.last_exit_reason << "\n";
        }
    }

    void nested_virtualization::validate_vm_state(const std::string& name)
    {
        auto it = m_nested_vms.find(name);
        if (it != m_nested_vms.end())
        {
            // Validate VM state
            // This could include:
            // - Memory integrity checks
            // - Register state validation
            // - Page table validation
        }
    }

    void nested_virtualization::destroy_nested_vm(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_nested_vms.erase(name);
    }

    std::shared_ptr<virtual_machine> nested_virtualization::get_nested_vm(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_nested_vms.find(name);
        if (it == m_nested_vms.end())
        {
            throw std::runtime_error("Nested VM with name " + name + " not found");
        }
        return it->second.vm;
    }

    bool nested_virtualization::is_nested_vm(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_nested_vms.find(name) != m_nested_vms.end();
    }

    std::vector<std::string> nested_virtualization::get_nested_vm_names() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::string> names;
        for (const auto& pair : m_nested_vms)
        {
            names.push_back(pair.first);
        }
        return names;
    }

    void nested_virtualization::setup_ept(uint8_t* guest_physical, uint8_t* host_physical)
    {
        // Setup Extended Page Tables for nested virtualization
        // This is a simplified implementation and would need to be adapted
        // to specific hardware virtualization features
    }

    void nested_virtualization::setup_vmcs(std::shared_ptr<virtual_machine> guest_vm, 
                                         std::shared_ptr<virtual_machine> host_vm)
    {
        // Setup Virtual Machine Control Structure for nested virtualization
        // This is a simplified implementation and would need to be adapted
        // to specific hardware virtualization features
    }

    void nested_virtualization::handle_nested_page_fault(uint8_t* guest_physical, uint8_t* host_physical)
    {
        // Handle nested page faults by mapping guest physical addresses to host physical addresses
        // This is a simplified implementation and would need to be adapted
        // to specific hardware virtualization features
    }
} 