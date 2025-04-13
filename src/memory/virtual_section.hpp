#pragma once

// STD
#include <vector>
#include <memory>

namespace vm
{
	class virtual_section
	{
	public:
		virtual_section(std::uint8_t* base) : m_base(base) {}

		using page_list_t = std::vector<std::unique_ptr<std::uint8_t[]>>;

		
		template <size_t N>
		void add()
		{
			this->pages().emplace_back(std::make_unique<std::uint8_t[]>(N));
		}

		
		std::uint8_t* get(size_t index);
		
		
		size_t count();

		
		std::uint8_t*& base();

	private:
		
		page_list_t& pages();
		page_list_t m_pages;

		std::uint8_t* m_base;
	};
}