#pragma once

// STD
#include <vector>
#include <initializer_list>

// x86
#include "instruction.hpp"

namespace x86
{
	class disassembler
	{
	public:
		template <class T, size_t N>
		disassembler(T(&buffer)[N]) : m_buffer(buffer, buffer + N) {}

		disassembler(std::vector<std::uint8_t> buffer) : m_buffer(buffer) {}

		
		std::uint8_t* buffer();
		size_t buffer_size();

		
		template <class T, class Fn>
		void iterate(T offset, Fn callback)
		{
			for (auto instruction_pointer = this->buffer() + offset; 
				instruction_pointer < this->buffer() + this->buffer_size();)
			{
				x86::instruction instr{};

				auto start = instruction_pointer;

				
				if (!instr.prefix_initialised())
				{
					
					x86::disassembler::handle_prefix(instr, instruction_pointer);
				}

				
				if (!instr.rex_initialised())
				{
					// HANDLE REX PREFIX
					x86::disassembler::handle_rex(instr, instruction_pointer);
				}

				
				if (!instr.opcode_initialised())
				{
					x86::disassembler::handle_opcode(instr, instruction_pointer);
				}

				
				if (!instr.operand_initialised())
				{
					x86::disassembler::handle_operand(instr, instruction_pointer);
				}

				
				instr.size() = static_cast<std::uint8_t>(instruction_pointer - start);

				
				callback(instr, instruction_pointer);
			}
		}

	private:

		
		static void handle_opcode(x86::instruction& instr, std::uint8_t*& buffer);
		static void handle_operand(x86::instruction& instr, std::uint8_t*& buffer);
		static void handle_prefix(x86::instruction& instr, std::uint8_t*& buffer);
		static void handle_rex(x86::instruction& instr, std::uint8_t*& buffer);

		std::vector<std::uint8_t> m_buffer;
	};
}