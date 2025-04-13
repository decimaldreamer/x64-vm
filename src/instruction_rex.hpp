#pragma once

// STD
#include <cstdint>
#include <string>

namespace x86
{
	
	struct rex
	{
		std::uint8_t b : 1;		
		std::uint8_t x : 1;		
		std::uint8_t r : 1;		
		std::uint8_t w	: 1;	
		std::uint8_t id : 4;	

		std::string to_string()
		{
			std::string str{};
			str.reserve(40);
			sprintf_s(str.data(), 40, "[B->%X, X->%X, R->%X, W->%X]", this->b, this->x, this->r, this->w);

			return str;
		}
	};
};