#include "nested_virtualization.hpp"
#include <stdexcept>
#include <algorithm>

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

        // Setup EPT and VMCS
        setup_ept(reinterpret_cast<uint8_t*>(state.memory.data()), 
                 parent_vm->get_memory().data());
        setup_vmcs(state.vm, parent_vm);

        m_nested_vms[name] = std::move(state);
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

    void nested_virtualization::handle_vmexit(uint8_t* guest_rip, uint8_t* host_rip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        // Handle VM exit by saving guest state and restoring host state
        // Implementation depends on specific hardware virtualization features
    }

    void nested_virtualization::handle_vmentry(uint8_t* guest_rip, uint8_t* host_rip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        // Handle VM entry by saving host state and restoring guest state
        // Implementation depends on specific hardware virtualization features
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