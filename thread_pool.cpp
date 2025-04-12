#include "thread_pool.hpp"

namespace vm
{
    thread_pool& thread_pool::get_instance()
    {
        static thread_pool instance;
        return instance;
    }

    thread_pool::thread_pool()
        : m_stop(false)
    {
        set_thread_count(std::thread::hardware_concurrency());
    }

    thread_pool::~thread_pool()
    {
        stop();
    }

    void thread_pool::set_thread_count(size_t count)
    {
        stop();

        m_stop = false;
        m_workers.clear();

        for (size_t i = 0; i < count; ++i)
        {
            m_workers.emplace_back([this] {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m_queue_mutex);
                        m_condition.wait(lock, [this] {
                            return m_stop || !m_tasks.empty();
                        });

                        if (m_stop && m_tasks.empty())
                        {
                            return;
                        }

                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    size_t thread_pool::get_thread_count() const
    {
        return m_workers.size();
    }

    void thread_pool::stop()
    {
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            m_stop = true;
        }
        m_condition.notify_all();

        for (std::thread& worker : m_workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }
} 