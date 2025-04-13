#pragma once

#include <vector>
#include <memory>
#include <string>

namespace x64_vm {
namespace optimization {

class PerformanceOptimizer {
public:
    PerformanceOptimizer();
    ~PerformanceOptimizer();

    // Performans metrikleri toplama
    void collect_metrics();
    
    // Kod optimizasyonu önerileri
    std::vector<std::string> suggest_optimizations();
    
    // Otomatik optimizasyon uygulama
    bool apply_optimizations();
    
    // Performans raporu oluşturma
    std::string generate_performance_report();

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

} // namespace optimization
} // namespace x64_vm 