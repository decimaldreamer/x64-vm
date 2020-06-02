#pragma once

// STD
#include <array>
#include <vector>

// x86
#include "disasm.hpp"
#include "register.hpp"

// VM
#include "virtual_stack.hpp"
#include "virtual_memory.hpp"


constexpr std::uint8_t vm_alignment = 0x8;
constexpr std::uint32_t vm_stack_size = 0x1000;


class virtual_machine
{
public:
	template <class T, size_t N>
	virtual_machine(T(&buffer)[N]) : m_buffer(buffer, buffer + N), m_stack(vm_alignment, vm_stack_size) {}
	virtual_machine(std::vector<std::uint8_t> buffer) : m_buffer(buffer), m_stack(vm_alignment, vm_stack_size) {}

	
	std::vector<std::uint8_t>& buffer();	
	x86::registr& context();				
	vm::virtual_memory& memory();			

	
	void handle_instruction(x86::instruction& instr);

	
	template <class T>
	void run(T offset)
	{
		
		auto disassembler = x86::disassembler(this->buffer());

		
		disassembler.iterate(offset, [this](x86::instruction instr, std::uint8_t*& instruction_pointer)
		{
			
			this->context().instruction_pointer() = instruction_pointer;

			try
			{
				
				this->handle_instruction(instr);
			}
			catch (std::exception exception)
			{
				
				global::console.log_error_indented<1>(exception.what());
			}
		

			
			instruction_pointer = this->context().instruction_pointer();
		});
	}

private:
	
	std::vector<std::uint8_t> m_buffer;

	
	x86::registr m_context;

	
	vm::virtual_stack m_stack;

	
	vm::virtual_memory m_memory;
};