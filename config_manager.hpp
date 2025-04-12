#pragma once

#include <string>
#include <map>
#include <variant>
#include <memory>
#include <mutex>
#include <filesystem>

namespace vm
{
    class config_manager
    {
    public:
        using config_value = std::variant<int, float, bool, std::string>;
        using config_map = std::map<std::string, config_value>;

        static config_manager& get_instance();

        void load_config(const std::filesystem::path& path);
        void save_config(const std::filesystem::path& path);
        void set_value(const std::string& key, const config_value& value);
        config_value get_value(const std::string& key, const config_value& default_value = {}) const;
        bool has_value(const std::string& key) const;
        void remove_value(const std::string& key);
        void clear();

        template<typename T>
        T get_value_as(const std::string& key, const T& default_value = T()) const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_config.find(key);
            if (it != m_config.end() && std::holds_alternative<T>(it->second))
            {
                return std::get<T>(it->second);
            }
            return default_value;
        }

    private:
        config_manager() = default;
        ~config_manager() = default;

        config_map m_config;
        mutable std::mutex m_mutex;
    };
} 