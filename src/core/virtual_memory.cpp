#include "virtual_memory.hpp"

bool vm::virtual_memory::allocate(std::uint8_t* address, size_t size)
{
	
	auto page_start = vm::virtual_memory::round_to_page(address);

	
	const auto search = this->address_space().find(page_start);
	if (search != this->address_space().end())
	{
		return false;
	}

	auto section = std::make_shared<vm::virtual_section>(page_start);

	const auto next_page = page_start + vm::virtual_memory::page_size;
	const auto exceeds_page = (address + size) >= next_page;

	if (!exceeds_page)
	{
	
		section.get()->add<vm::virtual_memory::page_size>();
		this->address_space().emplace(page_start, section);
	}
	else
	{
		auto bytes_left = size;
		auto page_index = 0;
		auto page_address = address;
		do
		{
			
			const auto page_offset = vm::virtual_memory::get_page_offset(page_address);

			
			const auto count_to_end = vm::virtual_memory::page_size - page_offset;
			auto bytes_allocated =  std::min(page_offset == 0 ? vm::virtual_memory::page_size : count_to_end, bytes_left);

			
			page_address = page_start + vm::virtual_memory::page_size * page_index;

			
			section.get()->add<vm::virtual_memory::page_size>();
			this->address_space().emplace(page_address, section);

			
			++page_index;

			bytes_left -= bytes_allocated;

		} while (bytes_left > 0);
	}

	return true;
}

bool vm::virtual_memory::free(std::uint8_t* address)
{
	
	auto page_start = vm::virtual_memory::round_to_page(address);

	const auto search = this->address_space().find(page_start);
	if (search == this->address_space().end())
	{
		
		auto section = search->second;
		auto page_count = section.get()->count();
		
		
		for (size_t page_index = 0; page_index < page_count; page_index++)
		{
			this->address_space().erase(page_start + vm::virtual_memory::page_size * page_index);
		}

		
		return true;
	}

	return false;
}

vm::virtual_memory::memory_map_t& vm::virtual_memory::address_space()
{
	return this->m_address_space;
}
