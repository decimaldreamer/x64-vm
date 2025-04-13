#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace x64_vm {
namespace graphics {

/**
 * @brief GPU Emulator Class
 * 
 * This class is used to virtualize basic graphics operations.
 * Supports OpenGL and DirectX APIs.
 */
class GPUEmulator {
public:
    GPUEmulator();
    ~GPUEmulator();

    /**
     * @brief Initializes the graphics context
     * @param width Screen width
     * @param height Screen height
     */
    void initialize(uint32_t width, uint32_t height);

    /**
     * @brief Processes a graphics command
     * @param command Command code
     * @param params Command parameters
     */
    void process_command(uint32_t command, const std::vector<uint32_t>& params);

    /**
     * @brief Gets the screen buffer
     * @return Screen buffer
     */
    std::vector<uint8_t> get_screen_buffer();

    /**
     * @brief Creates OpenGL context
     */
    void create_opengl_context();

    /**
     * @brief Creates DirectX context
     */
    void create_directx_context();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace graphics
} // namespace x64_vm 