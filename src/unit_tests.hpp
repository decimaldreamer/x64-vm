#pragma once

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <sstream>

namespace vm
{
    class test_case
    {
    public:
        test_case(const std::string& name, std::function<void()> test_func)
            : m_name(name)
            , m_test_func(test_func)
        {}

        void run()
        {
            try
            {
                m_test_func();
                m_passed = true;
            }
            catch (const std::exception& e)
            {
                m_error = e.what();
                m_passed = false;
            }
        }

        bool passed() const { return m_passed; }
        const std::string& name() const { return m_name; }
        const std::string& error() const { return m_error; }

    private:
        std::string m_name;
        std::function<void()> m_test_func;
        bool m_passed = false;
        std::string m_error;
    };

    class test_suite
    {
    public:
        static test_suite& get_instance();

        void add_test(const std::string& name, std::function<void()> test_func);
        void run_all();
        void run_test(const std::string& name);
        void clear();

        size_t get_total_tests() const;
        size_t get_passed_tests() const;
        size_t get_failed_tests() const;
        const std::vector<std::string>& get_failed_test_names() const;

    private:
        test_suite() = default;
        ~test_suite() = default;

        std::map<std::string, test_case> m_tests;
        std::vector<std::string> m_failed_tests;
        size_t m_total_tests = 0;
        size_t m_passed_tests = 0;
    };

    template<typename T>
    void assert_equal(const T& expected, const T& actual, const std::string& message = "")
    {
        if (expected != actual)
        {
            std::stringstream ss;
            ss << "Assertion failed: " << message << "\n";
            ss << "Expected: " << expected << "\n";
            ss << "Actual: " << actual;
            throw std::runtime_error(ss.str());
        }
    }

    template<typename T>
    void assert_not_equal(const T& expected, const T& actual, const std::string& message = "")
    {
        if (expected == actual)
        {
            std::stringstream ss;
            ss << "Assertion failed: " << message << "\n";
            ss << "Expected not equal to: " << expected << "\n";
            ss << "Actual: " << actual;
            throw std::runtime_error(ss.str());
        }
    }

    void assert_true(bool condition, const std::string& message = "");
    void assert_false(bool condition, const std::string& message = "");
    void assert_null(const void* ptr, const std::string& message = "");
    void assert_not_null(const void* ptr, const std::string& message = "");
    void assert_throws(std::function<void()> func, const std::string& message = "");
} 