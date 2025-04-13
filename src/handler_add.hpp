#pragma once


#include "instruction.hpp"

#include "virtual_machine.hpp"

namespace vm::handler::add
{
	namespace impl
	{
		template <class T>
		void handle_flags(virtual_machine* vm, T value, T previous_value, T addition)
		{
			
			vm->context().flags().zero = value == 0x000;

			
			vm->context().flags().overflow =
				numerical_helper::addition_overflows(value, addition);

			
			vm->context().flags().carry =
				numerical_helper::is_signed(value) != numerical_helper::is_signed(previous_value);

			
			vm->context().flags().sign = numerical_helper::is_signed(value) ? 1 : 0;

			
			const auto set_bit_count = numerical_helper::least_significant_bits(value);
			vm->context().flags().parity = set_bit_count % 2 == 0; 
		}
	}

	void handle(virtual_machine* vm, x86::instruction& instr);
}