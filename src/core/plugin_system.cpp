#include "plugin_system.hpp"
#include <dlfcn.h>
#include <filesystem>

namespace vm
{
    plugin_system& plugin_system::get_instance()
    {
        static plugin_system instance;
        return instance;
    }

    void plugin_system::load_plugin(const std::string& path)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle)
        {
            throw std::runtime_error("Failed to load plugin: " + std::string(dlerror()));
        }

        using create_plugin_t = plugin* (*)();
        auto create_plugin = reinterpret_cast<create_plugin_t>(dlsym(handle, "create_plugin"));
        if (!create_plugin)
        {
            dlclose(handle);
            throw std::runtime_error("Failed to find create_plugin symbol");
        }

        auto plugin = std::shared_ptr<vm::plugin>(create_plugin());
        m_plugins[plugin->get_name()] = plugin;
    }

    void plugin_system::unload_plugin(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_plugins.find(name);
        if (it != m_plugins.end())
        {
            it->second->shutdown();
            m_plugins.erase(it);
        }
    }

    void plugin_system::initialize_all()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (const auto& [_, plugin] : m_plugins)
        {
            plugin->initialize();
        }
    }

    void plugin_system::shutdown_all()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        for (const auto& [_, plugin] : m_plugins)
        {
            plugin->shutdown();
        }
    }

    void plugin_system::register_event_handler(const std::string& event, std::function<void()> handler)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_event_handlers[event].push_back(handler);
    }

    void plugin_system::unregister_event_handler(const std::string& event)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_event_handlers.erase(event);
    }

    void plugin_system::trigger_event(const std::string& event)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto it = m_event_handlers.find(event);
        if (it != m_event_handlers.end())
        {
            for (const auto& handler : it->second)
            {
                handler();
            }
        }
    }
} 