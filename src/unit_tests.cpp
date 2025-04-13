#include "unit_tests.hpp"

namespace vm
{
    test_suite& test_suite::get_instance()
    {
        static test_suite instance;
        return instance;
    }

    void test_suite::add_test(const std::string& name, std::function<void()> test_func)
    {
        m_tests[name] = test_case(name, test_func);
        m_total_tests++;
    }

    void test_suite::run_all()
    {
        m_passed_tests = 0;
        m_failed_tests.clear();

        for (auto& [name, test] : m_tests)
        {
            test.run();
            if (test.passed())
            {
                m_passed_tests++;
            }
            else
            {
                m_failed_tests.push_back(name);
            }
        }
    }

    void test_suite::run_test(const std::string& name)
    {
        auto it = m_tests.find(name);
        if (it != m_tests.end())
        {
            it->second.run();
            if (it->second.passed())
            {
                m_passed_tests++;
            }
            else
            {
                m_failed_tests.push_back(name);
            }
        }
    }

    void test_suite::clear()
    {
        m_tests.clear();
        m_failed_tests.clear();
        m_total_tests = 0;
        m_passed_tests = 0;
    }

    size_t test_suite::get_total_tests() const
    {
        return m_total_tests;
    }

    size_t test_suite::get_passed_tests() const
    {
        return m_passed_tests;
    }

    size_t test_suite::get_failed_tests() const
    {
        return m_failed_tests.size();
    }

    const std::vector<std::string>& test_suite::get_failed_test_names() const
    {
        return m_failed_tests;
    }

    void assert_true(bool condition, const std::string& message)
    {
        if (!condition)
        {
            std::stringstream ss;
            ss << "Assertion failed: " << message << "\n";
            ss << "Expected: true\n";
            ss << "Actual: false";
            throw std::runtime_error(ss.str());
        }
    }

    void assert_false(bool condition, const std::string& message)
    {
        if (condition)
        {
            std::stringstream ss;
            ss << "Assertion failed: " << message << "\n";
            ss << "Expected: false\n";
            ss << "Actual: true";
            throw std::runtime_error(ss.str());
        }
    }

    void assert_null(const void* ptr, const std::string& message)
    {
        if (ptr != nullptr)
        {
            std::stringstream ss;
            ss << "Assertion failed: " << message << "\n";
            ss << "Expected: nullptr\n";
            ss << "Actual: " << ptr;
            throw std::runtime_error(ss.str());
        }
    }

    void assert_not_null(const void* ptr, const std::string& message)
    {
        if (ptr == nullptr)
        {
            std::stringstream ss;
            ss << "Assertion failed: " << message << "\n";
            ss << "Expected: not nullptr\n";
            ss << "Actual: nullptr";
            throw std::runtime_error(ss.str());
        }
    }

    void assert_throws(std::function<void()> func, const std::string& message)
    {
        bool threw = false;
        try
        {
            func();
        }
        catch (...)
        {
            threw = true;
        }

        if (!threw)
        {
            std::stringstream ss;
            ss << "Assertion failed: " << message << "\n";
            ss << "Expected: exception to be thrown\n";
            ss << "Actual: no exception thrown";
            throw std::runtime_error(ss.str());
        }
    }
} 