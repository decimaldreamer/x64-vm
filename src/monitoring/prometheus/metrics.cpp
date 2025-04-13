#include <prometheus/counter.h>
#include <prometheus/gauge.h>
#include <prometheus/histogram.h>
#include <prometheus/registry.h>
#include <prometheus/exposer.h>
#include <memory>
#include <string>
#include "metrics.h"

namespace x64vm {
namespace monitoring {
namespace prometheus {

class MetricsCollector {
public:
    MetricsCollector(const std::string& bind_address = "0.0.0.0:8080") {
        // Create registry
        registry_ = std::make_shared<prometheus::Registry>();

        // Create metrics
        instruction_counter_ = &prometheus::BuildCounter()
            .Name("x64vm_instructions_total")
            .Help("Total number of instructions executed")
            .Register(*registry_)
            .Add({});

        memory_usage_gauge_ = &prometheus::BuildGauge()
            .Name("x64vm_memory_usage_bytes")
            .Help("Current memory usage in bytes")
            .Register(*registry_)
            .Add({});

        instruction_latency_histogram_ = &prometheus::BuildHistogram()
            .Name("x64vm_instruction_latency_seconds")
            .Help("Instruction execution latency in seconds")
            .Register(*registry_)
            .Add({}, prometheus::Histogram::BucketBoundaries{0.001, 0.01, 0.1, 1.0});

        // Start HTTP server
        exposer_ = std::make_unique<prometheus::Exposer>(bind_address);
        exposer_->RegisterCollectable(registry_);
    }

    void record_instruction() {
        instruction_counter_->Increment();
    }

    void update_memory_usage(size_t bytes) {
        memory_usage_gauge_->Set(bytes);
    }

    void record_instruction_latency(double seconds) {
        instruction_latency_histogram_->Observe(seconds);
    }

private:
    std::shared_ptr<prometheus::Registry> registry_;
    std::unique_ptr<prometheus::Exposer> exposer_;
    prometheus::Counter* instruction_counter_;
    prometheus::Gauge* memory_usage_gauge_;
    prometheus::Histogram* instruction_latency_histogram_;
};

} // namespace prometheus
} // namespace monitoring
} // namespace x64vm 