#pragma once

#include <string>
#include <vector>
#include <memory>

namespace x64_vm {
namespace security {

class SecurityManager {
public:
    SecurityManager();
    ~SecurityManager();

    // Memory security
    bool encrypt_memory();
    bool decrypt_memory();
    bool audit_security();

    // Enhanced security features
    bool enable_memory_protection();
    bool enable_code_signing();
    bool enable_secure_boot();
    bool enable_anti_tampering();
    bool enable_secure_debugging();

    // Security reporting
    std::string generate_security_report();
    std::vector<std::string> get_security_violations();

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

} // namespace security
} // namespace x64_vm 