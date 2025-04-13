#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include "service.grpc.pb.h"

namespace x64vm {
namespace api {
namespace grpc {

class VirtualMachineServiceImpl final : public VirtualMachineService::Service {
public:
    grpc::Status Execute(grpc::ServerContext* context,
                        const ExecuteRequest* request,
                        ExecuteResponse* response) override {
        try {
            auto result = execute_code(request->code(), request->parameters());
            response->set_result(result);
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            response->set_error(e.what());
            return grpc::Status::OK;
        }
    }

    grpc::Status GetStatus(grpc::ServerContext* context,
                          const StatusRequest* request,
                          StatusResponse* response) override {
        response->set_status("running");
        response->set_version("1.0.0");
        response->set_uptime(get_uptime());
        
        if (request->detailed()) {
            auto details = get_status_details();
            for (const auto& [key, value] : details) {
                (*response->mutable_details())[key] = value;
            }
        }
        
        return grpc::Status::OK;
    }

    grpc::Status GetMetrics(grpc::ServerContext* context,
                           const MetricsRequest* request,
                           MetricsResponse* response) override {
        response->set_instructions_executed(get_instructions_executed());
        response->set_memory_usage(get_memory_usage());
        response->set_cpu_usage(get_cpu_usage());
        
        if (request->include_performance()) {
            auto metrics = get_performance_metrics();
            for (const auto& [key, value] : metrics) {
                (*response->mutable_performance_metrics())[key] = value;
            }
        }
        
        if (request->include_system()) {
            auto metrics = get_system_metrics();
            for (const auto& [key, value] : metrics) {
                (*response->mutable_system_metrics())[key] = value;
            }
        }
        
        return grpc::Status::OK;
    }

    grpc::Status UpdateConfig(grpc::ServerContext* context,
                            const ConfigRequest* request,
                            ConfigResponse* response) override {
        try {
            update_config(request->settings());
            response->set_success(true);
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            response->set_success(false);
            response->set_error(e.what());
            return grpc::Status::OK;
        }
    }

    grpc::Status ClearCache(grpc::ServerContext* context,
                          const CacheRequest* request,
                          CacheResponse* response) override {
        try {
            int32_t cleared = clear_cache(request->clear_all(), request->keys());
            response->set_success(true);
            response->set_items_cleared(cleared);
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            response->set_success(false);
            return grpc::Status::OK;
        }
    }

    grpc::Status StreamMetrics(grpc::ServerContext* context,
                             const StreamRequest* request,
                             grpc::ServerWriter<MetricsResponse>* writer) override {
        try {
            while (!context->IsCancelled()) {
                MetricsResponse response;
                response.set_instructions_executed(get_instructions_executed());
                response.set_memory_usage(get_memory_usage());
                response.set_cpu_usage(get_cpu_usage());
                
                for (const auto& metric : request->metrics()) {
                    if (metric == "performance") {
                        auto metrics = get_performance_metrics();
                        for (const auto& [key, value] : metrics) {
                            (*response.mutable_performance_metrics())[key] = value;
                        }
                    } else if (metric == "system") {
                        auto metrics = get_system_metrics();
                        for (const auto& [key, value] : metrics) {
                            (*response.mutable_system_metrics())[key] = value;
                        }
                    }
                }
                
                writer->Write(response);
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(request->interval_ms())
                );
            }
            return grpc::Status::OK;
        } catch (const std::exception& e) {
            return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
        }
    }
};

} // namespace grpc
} // namespace api
} // namespace x64vm 