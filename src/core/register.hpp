#pragma once

// STD
#include <array>

namespace x86
{
	class registr
	{
	public:
		
		enum id : std::uint8_t
		{
			rax,	// 0000
			rcx,	// 0001
			rdx,	// 0010
			rbx,	// 0011
			rsp,	// 0100
			rbp,	// 0101
			rsi,	// 0110
			rdi,	// 0111
			r8,		// 1000
			r9,     // 1001
			r10,	// 1010
			r11,	// 1011
			r12,	// 1100
			r13,	// 1101
			r14,	// 1110
			r15,	// 1111
			END_OF_REGISTER
		};

		enum size : std::uint8_t
		{
			qword,
			dword,
			word,
			byte,
			END_OF_SIZE
		};

		
		union register_data
		{
			std::uint8_t*	ptr;
			uint64_t		qword;
			uint32_t		dword;
			uint16_t		word;
			uint8_t			byte;


			std::uint8_t* dword_ptr()
			{
				std::uint64_t tmp = this->dword;
				return reinterpret_cast<std::uint8_t*>(tmp);
			}
			std::uint8_t* word_ptr()
			{
				std::uint64_t tmp = this->word;
				return reinterpret_cast<std::uint8_t*>(tmp);
			}
		};

		
		struct flags_t
		{
			
			std::uint8_t carry : 1;

		private:
			
			std::uint8_t reserved_1 : 1;
		public:

			std::uint8_t parity : 1;

		private:
			
			std::uint8_t reserved_3 : 1; 
		public:

			std::uint8_t auxiliary_carry : 1; 

		private:
			
			std::uint8_t reserved_5 : 1; 
		public:

			std::uint8_t zero : 1;

			std::uint8_t sign : 1; 

			std::uint8_t trap : 1; 

			std::uint8_t interrupt : 1;

			std::uint8_t direction : 1; 

			std::uint8_t overflow : 1;

			std::uint8_t privilege_level : 2;

			std::uint8_t nested_task : 1;

		private:
			
			std::uint8_t reserved_15 : 1;
		public:

			
			std::uint8_t resume : 1;

			std::uint8_t vm : 1; 

			std::uint8_t alignment_check : 1;

			std::uint8_t virtual_interrupt : 1; 

			std::uint8_t virtual_interrupt_pending : 1;

			std::uint8_t identification : 1; 

		private:
			std::uint8_t reserved_22 : 1; 
			std::uint8_t reserved_23 : 1; 
			std::uint8_t reserved_24 : 1; 
			std::uint8_t reserved_25 : 1; 
			std::uint8_t reserved_26 : 1; 
			std::uint8_t reserved_27 : 1; 
			std::uint8_t reserved_28 : 1; 
			std::uint8_t reserved_29 : 1; 
			std::uint8_t reserved_30 : 1; 
			std::uint8_t reserved_31 : 1; 
		};

		
		x86::registr::register_data& get(std::uint8_t id);

		std::uint8_t*& instruction_pointer();

		
		flags_t& flags();

	
		using data_t = std::array<x86::registr::register_data, x86::registr::id::END_OF_REGISTER>;
		data_t& general_buffer();

		using register_names_t = std::array<std::string, x86::registr::size::END_OF_SIZE>;
		using register_name_map_t = std::array<register_names_t, x86::registr::id::END_OF_REGISTER>;
		static const register_name_map_t names;

	private:
		data_t m_general_buffer;
		std::uint8_t* m_instruction_pointer;
		flags_t m_flags;

	};
}