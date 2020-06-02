#pragma once

// STD
#include <array>
#include <vector>

namespace x86
{
	class prefix
	{
	public:
		prefix() : m_buffer() {}

		
		bool has(std::uint8_t prefix);

		
		void append(std::uint8_t prefix);

		
		using vector_iter_t = std::vector<std::uint8_t>::const_iterator;
		vector_iter_t begin();
		vector_iter_t end();

		
		static bool is(std::uint8_t prefix);

		
		using prefix_list_t = std::array<std::uint8_t, 14>;
		static const prefix_list_t list;

		
		enum id
		{
			
			LOCK = 0xF0,	
			REPNE = 0xF2,	
			REP = 0xF3,		
			BOUND = 0xF2,	

			CS_OVERRIDE = 0x2E,
			SS_OVERRIDE = 0x36,
			DS_OVERRIDE = 0x3E,
			ES_OVERRIDE = 0x26,
			FS_OVERRIDE = 0x64,
			GS_OVERRIDE = 0x65,

			
			BR_NOT_TAKEN = 0x2E,
			BR_TAKEN = 0x3E,

			
			OPERAND_SIZE_OVERRIDE = 0x66, 

			
			ADDRESS_SIZE_OVERRIDE = 0x67 
		};

	private:
		// ACCESSORS
		std::vector<std::uint8_t>& buffer();

		// PRIVATE MEMBERS

		// PREFIX BUFFER
		// MAYBE USE BITMAP?
		std::vector<std::uint8_t> m_buffer;
	};

	
};