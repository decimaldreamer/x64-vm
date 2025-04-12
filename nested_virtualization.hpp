#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <map>
#include "virtual_machine.hpp"

namespace vm
{
    class nested_virtualization
    {
    public:
        struct vm_state
        {
            std::shared_ptr<virtual_machine> vm;
            std::vector<uint8_t> memory;
            std::map<uint8_t*, uint8_t*> address_map;
        };

        static nested_virtualization& get_instance();

        void create_nested_vm(const std::string& name, std::shared_ptr<virtual_machine> parent_vm);
        void destroy_nested_vm(const std::string& name);
        std::shared_ptr<virtual_machine> get_nested_vm(const std::string& name);
        void handle_vmexit(uint8_t* guest_rip, uint8_t* host_rip);
        void handle_vmentry(uint8_t* guest_rip, uint8_t* host_rip);
        bool is_nested_vm(const std::string& name) const;
        std::vector<std::string> get_nested_vm_names() const;

    private:
        nested_virtualization() = default;
        ~nested_virtualization() = default;

        std::map<std::string, vm_state> m_nested_vms;
        std::mutex m_mutex;

        void setup_ept(uint8_t* guest_physical, uint8_t* host_physical);
        void setup_vmcs(std::shared_ptr<virtual_machine> guest_vm, std::shared_ptr<virtual_machine> host_vm);
        void handle_nested_page_fault(uint8_t* guest_physical, uint8_t* host_physical);
    };
} 