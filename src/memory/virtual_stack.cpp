#include "virtual_stack.hpp"
#include "numerical_helper.hpp"

namespace vm
{
	std::uint8_t* virtual_stack::stack_pointer()
	{
		
		return m_buffer.data() + this->stack_offset();
	}

	std::int32_t& virtual_stack::stack_offset()
	{
		
		return this->m_offset;
	}

	std::uint8_t virtual_stack::get_alignment()
	{
		return this->m_alignment;
	}

	std::uint32_t virtual_stack::get_size()
	{
		return this->m_size;
	}

	void virtual_stack::allocate(std::uint32_t size)
	{
	
		const auto delta = numerical_helper::round_to_multiple(static_cast<std::int32_t>(size), static_cast<std::int32_t>(this->get_alignment()));

		
		if (this->stack_offset() - delta < 0)
			throw std::exception("Stack overflow");

		
		this->stack_offset() -= delta;
	}

	void virtual_stack::release(std::uint32_t size)
	{
		
		const auto delta = numerical_helper::round_to_multiple(static_cast<std::int32_t>(size), static_cast<std::int32_t>(this->get_alignment()));

		
		if (this->stack_offset() + delta >= static_cast<std::int32_t>(this->get_size()))
			throw std::exception("Stack underflow");

		this->stack_offset() += delta;
	}
}

