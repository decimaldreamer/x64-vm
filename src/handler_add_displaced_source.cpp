// VM
#include "handler_add_displaced_source.hpp"
#include "handler_add.hpp"

// HELPER
#include "compiler_helper.hpp"
#include "numerical_helper.hpp"
#include "global.hpp"

void vm::handler::add::impl::displaced_source_zero(
	virtual_machine* vm,
	x86::instruction::modifier_data_t& modifier,
	x86::instruction& instr,
	uint64_t& addition)
{
	auto dest_size = x86::registr::size::qword;
	auto source_size = x86::registr::size::qword;

	auto& source_reg = vm->context().get(modifier.source_register);
	if (modifier.wide)
	{
		
		if (instr.prefix().has(x86::prefix::ADDRESS_SIZE_OVERRIDE))
		{
			addition = vm->memory().read<std::uint32_t>(source_reg.dword_ptr());
			source_size = x86::registr::size::dword;
		}
		else
		{

			addition = vm->memory().read<std::uint64_t>(source_reg.ptr);
		}
	}
	else
	{
		if (instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE))
		{
			addition = vm->memory().read<std::uint16_t>(source_reg.word_ptr());
			source_size = x86::registr::size::word;
			dest_size = x86::registr::size::word;
		}
		else
		{
			addition = vm->memory().read<std::uint32_t>(source_reg.dword_ptr());
			source_size = x86::registr::size::dword;
			dest_size = x86::registr::size::dword;
		}
	}

	
	const auto dest_reg_name_container = x86::registr::names[modifier.destination_register];
	const auto dest_reg_name = dest_reg_name_container[dest_size].c_str();

	const auto source_reg_name_container = x86::registr::names[modifier.source_register];
	const auto source_reg_name = source_reg_name_container[source_size].c_str();

	global::console.log_raw("[O]  ADD %s, [%s]", dest_reg_name, source_reg_name);
}

void vm::handler::add::impl::displaced_source_one(virtual_machine* vm,
	x86::instruction::modifier_data_t& modifier,
	x86::instruction& instr,
	uint64_t& addition)
{
	const auto displacement = instr.operand().get<std::uint8_t>(1);

	auto dest_size = x86::registr::size::qword;
	auto source_size = x86::registr::size::qword;

	auto& source_reg = vm->context().get(modifier.source_register);
	if (modifier.wide)
	{
		
		if (instr.prefix().has(x86::prefix::ADDRESS_SIZE_OVERRIDE))
		{
			addition = vm->memory().read<std::uint32_t>(source_reg.dword_ptr() + displacement);
			source_size = x86::registr::size::dword;
		}
		else
		{

			addition = vm->memory().read<std::uint64_t>(source_reg.ptr + displacement);
		}
	}
	else
	{
		
		if (instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE))
		{
			addition = vm->memory().read<std::uint16_t>(source_reg.word_ptr() + displacement);
			source_size = x86::registr::size::word;
			dest_size = x86::registr::size::word;
		}
		else
		{
			addition = vm->memory().read<std::uint32_t>(source_reg.dword_ptr() + displacement);
			source_size = x86::registr::size::dword;
			dest_size = x86::registr::size::dword;
		}
	}

	
	const auto dest_reg_name_container = x86::registr::names[modifier.destination_register];
	const auto dest_reg_name = dest_reg_name_container[dest_size].c_str();

	const auto source_reg_name_container = x86::registr::names[modifier.source_register];
	const auto source_reg_name = source_reg_name_container[source_size].c_str();

	global::console.log_raw("[O]  ADD %s, [%s+%02X]\n", dest_reg_name, source_reg_name, displacement);
}

void vm::handler::add::impl::displaced_source_two(
	virtual_machine* vm,
	x86::instruction::modifier_data_t& modifier,
	x86::instruction& instr,
	uint64_t& addition)
{
	const auto displacement = instr.operand().get<std::uint32_t>(1);

	auto dest_size = x86::registr::size::qword;
	auto source_size = x86::registr::size::qword;

	auto& source_reg = vm->context().get(modifier.source_register);
	if (modifier.wide)
	{
		
		if (instr.prefix().has(x86::prefix::ADDRESS_SIZE_OVERRIDE))
		{
			addition = vm->memory().read<std::uint32_t>(source_reg.dword_ptr() + displacement);
			source_size = x86::registr::size::dword;
		}
		else
		{

			addition = vm->memory().read<std::uint64_t>(source_reg.ptr + displacement);
		}
	}
	else
	{
		
		if (instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE))
		{
			addition = vm->memory().read<std::uint16_t>(source_reg.word_ptr() + displacement);
			source_size = x86::registr::size::word;
			dest_size = x86::registr::size::word;
		}
		else
		{
			addition = vm->memory().read<std::uint32_t>(source_reg.dword_ptr() + displacement);
			source_size = x86::registr::size::dword;
			dest_size = x86::registr::size::dword;
		}
	}

	

	const auto dest_reg_name_container = x86::registr::names[modifier.destination_register];
	const auto dest_reg_name = dest_reg_name_container[dest_size].c_str();

	const auto source_reg_name_container = x86::registr::names[modifier.source_register];
	const auto source_reg_name = source_reg_name_container[source_size].c_str();

	global::console.log_raw("[O] ADD %s, [%s+%08lX]\n", dest_reg_name, source_reg_name, displacement);
}

void vm::handler::add::impl::displaced_source_three(
	virtual_machine* vm,
	x86::instruction::modifier_data_t& modifier,
	x86::instruction& instr,
	uint64_t& addition)
{
	auto dest_size = x86::registr::size::qword;
	auto source_size = x86::registr::size::qword;

	auto& source_reg = vm->context().get(modifier.source_register);
	if (modifier.wide)
	{
		
		if (instr.prefix().has(x86::prefix::ADDRESS_SIZE_OVERRIDE))
		{
			addition = source_reg.dword;
			source_size = x86::registr::size::dword;
		}
		else
		{

			addition = source_reg.qword;
		}
	}
	else
	{
	
		if (instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE))
		{
			addition = source_reg.word;
			source_size = x86::registr::size::word;
			dest_size = x86::registr::size::word;
		}
		else
		{
			addition = source_reg.dword;
			source_size = x86::registr::size::dword;
			dest_size = x86::registr::size::dword;
		}
	}

	

	const auto dest_reg_name_container = x86::registr::names[modifier.destination_register];
	const auto dest_reg_name = dest_reg_name_container[dest_size].c_str();

	const auto source_reg_name_container = x86::registr::names[modifier.source_register];
	const auto source_reg_name = source_reg_name_container[source_size].c_str();

	global::console.log_raw("[O] ADD %s, %s\n", dest_reg_name, source_reg_name);
}

void vm::handler::add::displaced_source(virtual_machine* vm, x86::instruction& instr)
{
	
	auto modifier = instr.get_modifier(0);

	std::uint64_t addition = 0;
	switch (modifier.mode)
	{
	case 0:
		
		vm::handler::add::impl::displaced_source_zero(vm, modifier, instr, addition);
		break;

	case 1:
		
		vm::handler::add::impl::displaced_source_one(vm, modifier, instr, addition);
		break;

	case 2:
		
		vm::handler::add::impl::displaced_source_two(vm, modifier, instr, addition);
		break;

	case 3:
		
		vm::handler::add::impl::displaced_source_three(vm, modifier, instr, addition);
		break;
	}

	
	auto& destination_reg = vm->context().get(modifier.destination_register);
	if (modifier.wide)
	{
		
		const auto previous_value = destination_reg.qword;

		
		destination_reg.qword += addition;

		
		vm::handler::add::impl::handle_flags(
			vm, 
			destination_reg.qword, 
			previous_value, 
			addition);
	}
	else
	{
		
		if (instr.prefix().has(x86::prefix::OPERAND_SIZE_OVERRIDE))
		{
			
			const auto previous_value = destination_reg.word;

			
			const auto casted_addition = static_cast<std::uint16_t>(addition);
			destination_reg.word += casted_addition;

			
			vm::handler::add::impl::handle_flags(
				vm,
				destination_reg.word,
				previous_value,
				casted_addition);

		}
		else
		{
			
			const auto previous_value = destination_reg.dword;

			
			const auto casted_addition = static_cast<std::uint32_t>(addition);
			destination_reg.dword += casted_addition;

			
			vm::handler::add::impl::handle_flags(
				vm,
				destination_reg.dword,
				previous_value,
				casted_addition);
		}
	}
}
