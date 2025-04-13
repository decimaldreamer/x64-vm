#include "handler_unknown.hpp"

void vm::handler::unknown(x86::instruction& instr)
{
	
	printf("Size: [%02X] ", instr.size());

	
	printf("Prefix: [");
	for (const auto prefix : instr.prefix())
		printf("%2X", prefix);
	printf("] ");

	
	if (instr.rex().has_value())
	{
		auto rex_value = instr.rex().value();
		printf("REX: %s ", rex_value.to_string().c_str());
	}

	
	printf("Opcode: [");
	for (const auto& opcode : instr.opcode().buffer())
		printf("%02X", opcode);
	printf("] ");

	printf("Operand: [");
	for (const auto& operand : instr.operand().buffer())
		printf("%02X", operand);
	printf("]");
}
