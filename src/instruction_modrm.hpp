#pragma once

// STD
#include <cstdint>

namespace x86
{
	// REGISTER OPERAND
	struct modrm
	{
		std::uint8_t rm : 3;	
		std::uint8_t reg : 3;	
		std::uint8_t mode : 2;	

		
		// MODE 0: [REG]
		// MODE 1: [REG]+disp8
		// MODE 2: [REG]+disp32
		// MODE 3: REG

		
		std::uint8_t data_size()
		{
			switch (this->mode)
			{
			// 8-bit
			case 1:
			{
				return 1;
			}
			// 32-bit 
			case 2:
			{
				return 4;
			}
			
			default:
				return 0;
			}
		}

		
		std::string mode_to_string()
		{
			const char* modes[] = {
				"Dereference",
				"8-bit displacement",
				"32-bit displacement",
				"No displacement"
			};

			if (this->mode > 3)
				throw std::exception("Invalid modrm mode");

			return std::string( modes[this->mode] );
		}
	};

	
	struct modrm_e
	{
		modrm_e() {}

		modrm_e(x86::modrm operand, x86::rex rex)
		{
			this->rm = operand.rm | (rex.b << 3);
			this->reg = operand.reg | (rex.r << 3);
			this->mode = operand.mode;
		}

		std::uint8_t rm : 4;
		std::uint8_t reg : 4;
		std::uint8_t mode : 2;
	};
}