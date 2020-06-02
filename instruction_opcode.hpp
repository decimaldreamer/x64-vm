#pragma once

// STD
#include <cstdint>
#include <map>
#include <vector>


namespace x86
{
	class opcode
	{
	public:
		
		opcode(std::vector<std::uint8_t> opcode) : m_opcode(opcode) {}

		
		template <class T, size_t N>
		opcode(T(&opcode)[N]) : m_opcode(opcode) {}

		
		opcode() : m_opcode() {}

		
		std::vector<std::uint8_t>& buffer();

		
		template <class T>
		T as()
		{
			const auto size = this->buffer().size();
		
			
			if (sizeof(T) > size)
			{
				
				T value{};
		
				for (size_t i = 0; i < size; i++)
					value += this->buffer().at(i) << (8 * i);
		
				return value;
			}
		
			return *reinterpret_cast<T*>(this->buffer().data());
		}

#pragma region Opcodes
#pragma region ADD
		
		constexpr static std::uint32_t add_byte = 0x00;
		constexpr static std::uint32_t add_reg = 0x01;
		constexpr static std::uint32_t add_displaced_byte = 0x02;
		constexpr static std::uint32_t add_displaced_source = 0x03;			
		constexpr static std::uint32_t add_al = 0x04;
		constexpr static std::uint32_t add_rax = 0x05;
		constexpr static std::uint32_t add_wide = 0x80;
		constexpr static std::uint32_t add_displaced_destination = 0x81;	
		constexpr static std::uint32_t add_byte_extended = 0x83;
#pragma endregion
		// INC
		// DEC
		// SUB
		// DIV
		// MUL
		// POP
		// PUSH
		// JMP
		// CALL
		// RET
		// MOV
#pragma endregion

		
		using name_map_t = std::map<std::uint32_t, std::string>;
		const static name_map_t names;

	private:
		
		std::vector<std::uint8_t> m_opcode;

	};
}