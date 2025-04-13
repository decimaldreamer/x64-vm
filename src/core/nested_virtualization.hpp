#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <map>
#include <mutex>
#include <functional>
#include "virtual_machine.hpp"

namespace vm
{
    class nested_virtualization
    {
    public:
        static nested_virtualization& get_instance();

        // Advanced VM state management
        struct vm_state
        {
            std::shared_ptr<virtual_machine> vm;
            std::vector<uint8_t> memory;
            std::map<uint64_t, uint64_t> address_translation;
            std::vector<std::function<void()>> exit_handlers;
            std::vector<std::function<void()>> entry_handlers;
            bool is_paused;
            uint64_t last_exit_reason;
            std::vector<uint8_t> saved_state;
        };

        // Enhanced VM management
        void create_nested_vm(const std::string& name, std::shared_ptr<virtual_machine> parent_vm);
        void destroy_nested_vm(const std::string& name);
        std::shared_ptr<virtual_machine> get_nested_vm(const std::string& name);
        
        // Advanced VM control
        void pause_nested_vm(const std::string& name);
        void resume_nested_vm(const std::string& name);
        void save_nested_vm_state(const std::string& name);
        void restore_nested_vm_state(const std::string& name);
        
        // Enhanced event handling
        void register_exit_handler(const std::string& name, std::function<void()> handler);
        void register_entry_handler(const std::string& name, std::function<void()> handler);
        void handle_vmexit(uint8_t* guest_rip, uint8_t* host_rip);
        void handle_vmentry(uint8_t* guest_rip, uint8_t* host_rip);
        
        // Advanced state queries
        bool is_nested_vm(const std::string& name) const;
        bool is_vm_paused(const std::string& name) const;
        uint64_t get_last_exit_reason(const std::string& name) const;
        std::vector<std::string> get_nested_vm_names() const;
        
        // Performance monitoring
        uint64_t get_vm_instruction_count(const std::string& name) const;
        uint64_t get_vm_memory_usage(const std::string& name) const;
        double get_vm_cpu_usage(const std::string& name) const;

    private:
        nested_virtualization() = default;
        ~nested_virtualization() = default;
        
        // Advanced memory management
        void setup_ept(uint8_t* guest_physical, uint8_t* host_physical);
        void setup_vmcs(std::shared_ptr<virtual_machine> guest_vm, 
                       std::shared_ptr<virtual_machine> host_vm);
        void handle_nested_page_fault(uint8_t* guest_physical, uint8_t* host_physical);
        
        // Performance optimization
        void optimize_memory_access();
        void prefetch_instructions();
        void cache_translations();
        
        // Debug support
        void log_vm_state(const std::string& name);
        void validate_vm_state(const std::string& name);
        
        std::map<std::string, vm_state> m_nested_vms;
        mutable std::mutex m_mutex;
    };
} 