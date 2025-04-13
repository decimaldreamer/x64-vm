#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace x64_vm {
namespace jit {

/**
 * @brief JIT (Just-In-Time) Compiler Class
 * 
 * This class is used to compile frequently used code blocks into native machine code
 * to improve performance.
 */
class JITCompiler {
public:
    JITCompiler();
    ~JITCompiler();

    /**
     * @brief Compiles the given code block
     * @param code Code block to compile
     * @return Address of the compiled function
     */
    void* compile(const std::vector<uint8_t>& code);

    /**
     * @brief Frees the compiled code
     * @param compiled_code Address of the code to free
     */
    void free(void* compiled_code);

    /**
     * @brief Clears the cache
     */
    void clear_cache();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace jit
} // namespace x64_vm 