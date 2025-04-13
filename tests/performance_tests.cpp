#include <gtest/gtest.h>
#include "../optimization/performance_optimizer.hpp"
#include "../core/virtual_machine.hpp"

namespace x64_vm {
namespace tests {

class PerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        vm = std::make_unique<core::VirtualMachine>();
        optimizer = std::make_unique<optimization::PerformanceOptimizer>();
    }

    std::unique_ptr<core::VirtualMachine> vm;
    std::unique_ptr<optimization::PerformanceOptimizer> optimizer;
};

TEST_F(PerformanceTest, MemoryAllocationPerformance) {
    const size_t TEST_SIZE = 1024 * 1024; // 1MB
    auto start = std::chrono::high_resolution_clock::now();
    vm->allocate_memory(TEST_SIZE);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    EXPECT_LT(duration.count(), 1000); // 1ms'den az sürmeli
}

TEST_F(PerformanceTest, OptimizationSuggestions) {
    optimizer->collect_metrics();
    auto suggestions = optimizer->suggest_optimizations();
    EXPECT_FALSE(suggestions.empty());
}

TEST_F(PerformanceTest, AutomaticOptimization) {
    bool result = optimizer->apply_optimizations();
    EXPECT_TRUE(result);
}

} // namespace tests
} // namespace x64_vm 