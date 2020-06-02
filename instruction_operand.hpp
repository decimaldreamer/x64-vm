#pragma once

// STD
#include <map>
#include <vector>
#include <tuple>

// x86
#include "instruction_opcode.hpp"

namespace x86
{
	class operand
	{
public:
		
		operand(std::vector<std::uint8_t> operand) : m_operand(operand) {}

		
		template <class T, size_t N>
		operand(T(&operand)[N]) : m_operand(operand) {}

		
		operand() : m_operand() {}

		
		std::vector<std::uint8_t>& buffer();

		
		template <typename T>
		T get(size_t index)
		{
			if (index + sizeof(T) > this->buffer().size())
			{
				throw std::exception("Operand size mismatch");
			}

			return *reinterpret_cast<T*>(this->buffer().data() + index);
		}

	private:
		
		template <typename T>
		T get_dynamic(size_t& index)
		{
			const T value = get<T>(index);
			index += sizeof(T);
			return value;
		}
	public:
		
		template <typename ... T>
		std::tuple<T...> get_multiple(size_t index)
		{
			return std::make_tuple(get_dynamic<T>(index)...);
		}

		struct operand_info
		{
			//operand_info(bool new_modrm, std::uint8_t new_size) : modrm(new_modrm), size(new_size) {}
			bool modrm;
			std::uint8_t size;
		};

		
		using operand_map_t = std::map<std::uint32_t, x86::operand::operand_info>;
		const static operand_map_t info;

	private:
		std::vector<std::uint8_t> m_operand;
	};
}