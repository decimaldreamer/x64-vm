#pragma once

#include <stdexcept>
#include <string>

namespace vm
{
    class vm_exception : public std::exception
    {
    public:
        explicit vm_exception(const std::string& message) : m_message(message) {}
        const char* what() const noexcept override { return m_message.c_str(); }

    private:
        std::string m_message;
    };

    class page_fault_exception : public vm_exception
    {
    public:
        explicit page_fault_exception(const std::string& message) : vm_exception(message) {}
    };

    class invalid_instruction_exception : public vm_exception
    {
    public:
        explicit invalid_instruction_exception(const std::string& message) : vm_exception(message) {}
    };

    class register_access_exception : public vm_exception
    {
    public:
        explicit register_access_exception(const std::string& message) : vm_exception(message) {}
    };

    class memory_access_exception : public vm_exception
    {
    public:
        explicit memory_access_exception(const std::string& message) : vm_exception(message) {}
    };

    class stack_overflow_exception : public vm_exception
    {
    public:
        explicit stack_overflow_exception(const std::string& message) : vm_exception(message) {}
    };

    class stack_underflow_exception : public vm_exception
    {
    public:
        explicit stack_underflow_exception(const std::string& message) : vm_exception(message) {}
    };
} 