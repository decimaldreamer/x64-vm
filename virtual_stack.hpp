#pragma once

// STD
#include <vector>

namespace vm
{
	class virtual_stack
	{
	public:
		
		virtual_stack(std::uint8_t alignment, std::uint32_t size) : m_alignment(alignment), m_size(size), m_buffer(m_size), m_offset(static_cast<std::int32_t>(m_size) - 1) { }

		
		std::uint8_t* stack_pointer();

		
		std::int32_t& stack_offset();

		//  0x08
		std::uint8_t get_alignment();

		// PAGE_SIZE(0x1000) 
		std::uint32_t get_size();

		// E n='size' 
		void allocate(std::uint32_t size);

		//  n='size' 
		void release(std::uint32_t size);

		// 
		template <class T>
		void push(T value)
		{
			
			this->allocate(sizeof(T));

			
			*reinterpret_cast<T*>(this->stack_pointer()) = value;
		}

		
		template <class T>
		T pop()
		{
			
			const T value = *reinterpret_cast<T*>(this->stack_pointer());

			
			this->release(sizeof(T));

			
			return value;
		}

	private:

		
		const std::uint8_t			m_alignment;
		const std::uint32_t			m_size;
		std::vector<std::uint8_t>	m_buffer;
		std::int32_t				m_offset;
	};
}
