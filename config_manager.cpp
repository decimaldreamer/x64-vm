#include "config_manager.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace vm
{
    config_manager& config_manager::get_instance()
    {
        static config_manager instance;
        return instance;
    }

    void config_manager::load_config(const std::filesystem::path& path)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!std::filesystem::exists(path))
        {
            return;
        }

        std::ifstream file(path);
        nlohmann::json json;
        file >> json;

        for (const auto& [key, value] : json.items())
        {
            if (value.is_number_integer())
            {
                m_config[key] = value.get<int>();
            }
            else if (value.is_number_float())
            {
                m_config[key] = value.get<float>();
            }
            else if (value.is_boolean())
            {
                m_config[key] = value.get<bool>();
            }
            else if (value.is_string())
            {
                m_config[key] = value.get<std::string>();
            }
        }
    }

    void config_manager::save_config(const std::filesystem::path& path)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        nlohmann::json json;
        for (const auto& [key, value] : m_config)
        {
            std::visit([&json, &key](auto&& arg) {
                json[key] = arg;
            }, value);
        }

        std::ofstream file(path);
        file << json.dump(4);
    }

    void config_manager::set_value(const std::string& key, const config_value& value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_config[key] = value;
    }

    config_manager::config_value config_manager::get_value(const std::string& key, const config_value& default_value) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_config.find(key);
        return it != m_config.end() ? it->second : default_value;
    }

    bool config_manager::has_value(const std::string& key) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_config.find(key) != m_config.end();
    }

    void config_manager::remove_value(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_config.erase(key);
    }

    void config_manager::clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_config.clear();
    }
} 