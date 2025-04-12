#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <mutex>

namespace vm
{
    class plugin
    {
    public:
        virtual ~plugin() = default;
        virtual void initialize() = 0;
        virtual void shutdown() = 0;
        virtual std::string get_name() const = 0;
        virtual std::string get_version() const = 0;
    };

    class plugin_system
    {
    public:
        static plugin_system& get_instance();

        void load_plugin(const std::string& path);
        void unload_plugin(const std::string& name);
        void initialize_all();
        void shutdown_all();

        template<typename T>
        std::shared_ptr<T> get_plugin(const std::string& name)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_plugins.find(name);
            if (it != m_plugins.end())
            {
                return std::dynamic_pointer_cast<T>(it->second);
            }
            return nullptr;
        }

        void register_event_handler(const std::string& event, std::function<void()> handler);
        void unregister_event_handler(const std::string& event);
        void trigger_event(const std::string& event);

    private:
        plugin_system() = default;
        ~plugin_system() = default;

        std::map<std::string, std::shared_ptr<plugin>> m_plugins;
        std::map<std::string, std::vector<std::function<void()>>> m_event_handlers;
        std::mutex m_mutex;
    };
} 