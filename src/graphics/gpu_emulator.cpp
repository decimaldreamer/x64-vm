#include "gpu_emulator.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
#include <d3d11.h>
#include <memory>
#include <vector>
#include <stdexcept>

namespace x64_vm {
namespace graphics {

class GPUEmulator::Impl {
public:
    Impl() : width_(0), height_(0), opengl_context_(nullptr), directx_context_(nullptr) {}

    ~Impl() {
        if (opengl_context_) {
            wglDeleteContext(opengl_context_);
        }
        if (directx_context_) {
            directx_context_->Release();
        }
    }

    void initialize(uint32_t width, uint32_t height) {
        width_ = width;
        height_ = height;
        screen_buffer_.resize(width * height * 4); // RGBA format
    }

    void process_command(uint32_t command, const std::vector<uint32_t>& params) {
        switch (command) {
            case 0x01: // Clear screen
                clear_screen(params[0]); // Color
                break;
            case 0x02: // Draw pixel
                draw_pixel(params[0], params[1], params[2]); // x, y, color
                break;
            case 0x03: // Draw line
                draw_line(params[0], params[1], params[2], params[3], params[4]); // x1, y1, x2, y2, color
                break;
            case 0x04: // Draw rectangle
                draw_rectangle(params[0], params[1], params[2], params[3], params[4]); // x, y, w, h, color
                break;
            default:
                throw std::runtime_error("Unknown graphics command");
        }
    }

    std::vector<uint8_t> get_screen_buffer() {
        return screen_buffer_;
    }

    void create_opengl_context() {
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,
            8,
            0,
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };

        HDC hdc = GetDC(GetDesktopWindow());
        int pixel_format = ChoosePixelFormat(hdc, &pfd);
        SetPixelFormat(hdc, pixel_format, &pfd);
        opengl_context_ = wglCreateContext(hdc);
        wglMakeCurrent(hdc, opengl_context_);

        // Initialize GLEW
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Failed to initialize GLEW");
        }
    }

    void create_directx_context() {
        D3D_FEATURE_LEVEL feature_levels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };

        ID3D11Device* device;
        ID3D11DeviceContext* context;
        D3D_FEATURE_LEVEL feature_level;

        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            feature_levels,
            ARRAYSIZE(feature_levels),
            D3D11_SDK_VERSION,
            &device,
            &feature_level,
            &context
        );

        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create DirectX device");
        }

        directx_context_ = context;
        device->Release();
    }

private:
    void clear_screen(uint32_t color) {
        for (size_t i = 0; i < screen_buffer_.size(); i += 4) {
            screen_buffer_[i] = (color >> 24) & 0xFF; // R
            screen_buffer_[i + 1] = (color >> 16) & 0xFF; // G
            screen_buffer_[i + 2] = (color >> 8) & 0xFF; // B
            screen_buffer_[i + 3] = color & 0xFF; // A
        }
    }

    void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
        if (x >= width_ || y >= height_) return;
        size_t index = (y * width_ + x) * 4;
        screen_buffer_[index] = (color >> 24) & 0xFF;
        screen_buffer_[index + 1] = (color >> 16) & 0xFF;
        screen_buffer_[index + 2] = (color >> 8) & 0xFF;
        screen_buffer_[index + 3] = color & 0xFF;
    }

    void draw_line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color) {
        // Bresenham's line algorithm
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            draw_pixel(x1, y1, color);
            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }

    void draw_rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
        for (uint32_t i = x; i < x + w; ++i) {
            for (uint32_t j = y; j < y + h; ++j) {
                draw_pixel(i, j, color);
            }
        }
    }

    uint32_t width_;
    uint32_t height_;
    std::vector<uint8_t> screen_buffer_;
    HGLRC opengl_context_;
    ID3D11DeviceContext* directx_context_;
};

GPUEmulator::GPUEmulator() : impl_(std::make_unique<Impl>()) {}
GPUEmulator::~GPUEmulator() = default;

void GPUEmulator::initialize(uint32_t width, uint32_t height) {
    impl_->initialize(width, height);
}

void GPUEmulator::process_command(uint32_t command, const std::vector<uint32_t>& params) {
    impl_->process_command(command, params);
}

std::vector<uint8_t> GPUEmulator::get_screen_buffer() {
    return impl_->get_screen_buffer();
}

void GPUEmulator::create_opengl_context() {
    impl_->create_opengl_context();
}

void GPUEmulator::create_directx_context() {
    impl_->create_directx_context();
}

} // namespace graphics
} // namespace x64_vm 