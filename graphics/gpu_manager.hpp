#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace x64_vm {
namespace graphics {

class GPUManager {
public:
    GPUManager();
    ~GPUManager();

    // Basic GPU operations
    bool initialize_gpu();
    bool shutdown_gpu();
    bool allocate_gpu_memory(size_t size);
    bool free_gpu_memory();

    // Extended GPU support
    bool enable_directx_support();
    bool enable_opengl_support();
    bool enable_vulkan_support();
    bool enable_ray_tracing();
    bool enable_ai_acceleration();

    // GPU optimizations
    bool enable_shader_caching();
    bool enable_texture_compression();
    bool enable_async_compute();
    bool enable_multi_gpu_support();

    // GPU status and performance
    std::string get_gpu_info();
    std::string get_gpu_utilization();
    std::string get_gpu_temperature();
    std::string get_gpu_memory_usage();

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

} // namespace graphics
} // namespace x64_vm 