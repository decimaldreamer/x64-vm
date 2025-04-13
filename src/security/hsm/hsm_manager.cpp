#include <memory>
#include <string>
#include <vector>
#include "hsm_manager.h"

namespace x64vm {
namespace security {
namespace hsm {

class HSMManager {
public:
    HSMManager(const std::string& hsm_library_path) {
        // Load HSM library
        hsm_library_ = dlopen(hsm_library_path.c_str(), RTLD_NOW);
        if (!hsm_library_) {
            throw std::runtime_error("Failed to load HSM library");
        }

        // Initialize HSM
        initialize_hsm();
    }

    ~HSMManager() {
        if (hsm_library_) {
            dlclose(hsm_library_);
        }
    }

    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) {
        // Encrypt data using HSM
        std::vector<uint8_t> encrypted_data;
        encrypted_data.resize(data.size() + 32); // Add space for padding and IV

        size_t encrypted_size = 0;
        if (!hsm_encrypt(data.data(), data.size(),
                        encrypted_data.data(), &encrypted_size)) {
            throw std::runtime_error("HSM encryption failed");
        }

        encrypted_data.resize(encrypted_size);
        return encrypted_data;
    }

    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& encrypted_data) {
        // Decrypt data using HSM
        std::vector<uint8_t> decrypted_data;
        decrypted_data.resize(encrypted_data.size());

        size_t decrypted_size = 0;
        if (!hsm_decrypt(encrypted_data.data(), encrypted_data.size(),
                        decrypted_data.data(), &decrypted_size)) {
            throw std::runtime_error("HSM decryption failed");
        }

        decrypted_data.resize(decrypted_size);
        return decrypted_data;
    }

    std::vector<uint8_t> sign(const std::vector<uint8_t>& data) {
        // Sign data using HSM
        std::vector<uint8_t> signature;
        signature.resize(256); // RSA-2048 signature size

        size_t signature_size = 0;
        if (!hsm_sign(data.data(), data.size(),
                     signature.data(), &signature_size)) {
            throw std::runtime_error("HSM signing failed");
        }

        signature.resize(signature_size);
        return signature;
    }

    bool verify(const std::vector<uint8_t>& data,
               const std::vector<uint8_t>& signature) {
        // Verify signature using HSM
        return hsm_verify(data.data(), data.size(),
                         signature.data(), signature.size());
    }

private:
    void initialize_hsm() {
        // Initialize HSM connection
        if (!hsm_initialize()) {
            throw std::runtime_error("HSM initialization failed");
        }

        // Load keys
        if (!hsm_load_keys()) {
            throw std::runtime_error("Failed to load HSM keys");
        }
    }

    void* hsm_library_;
    
    // HSM function pointers
    bool (*hsm_initialize)() = nullptr;
    bool (*hsm_load_keys)() = nullptr;
    bool (*hsm_encrypt)(const uint8_t*, size_t, uint8_t*, size_t*) = nullptr;
    bool (*hsm_decrypt)(const uint8_t*, size_t, uint8_t*, size_t*) = nullptr;
    bool (*hsm_sign)(const uint8_t*, size_t, uint8_t*, size_t*) = nullptr;
    bool (*hsm_verify)(const uint8_t*, size_t, const uint8_t*, size_t) = nullptr;
};

} // namespace hsm
} // namespace security
} // namespace x64vm 