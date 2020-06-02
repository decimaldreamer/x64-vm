// STD
#include <algorithm>

// x86
#include "disasm.hpp"
#include "instruction_prefix.hpp"
#include "instruction_rex.hpp"
#include "instruction_opcode.hpp"
#include "instruction_operand.hpp"
#include "instruction_modrm.hpp"

std::uint8_t* x86::disassembler::buffer()
{
	return this->m_buffer.data();
}

size_t x86::disassembler::buffer_size()
{
	return this->m_buffer.size();
}

void x86::disassembler::handle_opcode(x86::instruction& instr, std::uint8_t*& buffer)
{
	
	instr.opcode().buffer().emplace_back(*buffer);
	++buffer;

	
	instr.opcode_initialised() = true;
}

void x86::disassembler::handle_operand(x86::instruction& instr, std::uint8_t*& buffer)
{
	
	const auto search = x86::operand::info.find(instr.opcode().as<std::uint32_t>());

	
	if (search != x86::operand::info.end())
	{
		
		const auto info = search->second;
		auto size = info.size;

		
		if (info.modrm)
		{
			size += reinterpret_cast<x86::modrm*>(buffer)->data_size();
		}

		
		for (size_t i = 0; i < size; i++, buffer++)
			instr.operand().buffer().emplace_back(*buffer);
	}

	
	instr.operand_initialised() = true;
}

void x86::disassembler::handle_prefix(x86::instruction& instr, std::uint8_t*& buffer)
{
	
	for (size_t i = 0; i < 4; i++)
	{
		
		if (x86::prefix::is(*buffer))
		{
			
			instr.prefix().append(*buffer);
			++buffer;
			continue;
		}
		else
		{
			
			break;
		}
	}

	
	instr.prefix_initialised() = true;
}

void x86::disassembler::handle_rex(x86::instruction& instr, std::uint8_t*& buffer)
{
	
	auto rex = reinterpret_cast<x86::rex*>(buffer);

	// 0100, REX 
	if (rex->id == 0x04)
	{
		
		instr.rex() = *rex;

		
		++buffer;
	}

	instr.rex_initialised() = true;
}
