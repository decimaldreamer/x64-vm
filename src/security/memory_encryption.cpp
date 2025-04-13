#include "memory_encryption.hpp"
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <memory>
#include <vector>
#include <stdexcept>

namespace x64_vm {
namespace security {

class MemoryEncryption::Impl {
public:
    Impl() {
        // Initialize OpenSSL
        OpenSSL_add_all_algorithms();
        
        // Generate initial key
        key_.resize(32); // 256-bit key
        if (RAND_bytes(key_.data(), key_.size()) != 1) {
            throw std::runtime_error("Failed to generate random key");
        }

        // Initialize cipher context
        ctx_ = EVP_CIPHER_CTX_new();
        if (!ctx_) {
            throw std::runtime_error("Failed to create cipher context");
        }
    }

    ~Impl() {
        if (ctx_) {
            EVP_CIPHER_CTX_free(ctx_);
        }
        EVP_cleanup();
    }

    void encrypt(void* data, size_t size) {
        if (!data || size == 0) return;

        // Initialize encryption
        if (EVP_EncryptInit_ex(ctx_, EVP_aes_256_cbc(), nullptr, 
                             key_.data(), iv_.data()) != 1) {
            throw std::runtime_error("Failed to initialize encryption");
        }

        // Encrypt data
        int outlen;
        if (EVP_EncryptUpdate(ctx_, 
                            static_cast<unsigned char*>(data),
                            &outlen,
                            static_cast<const unsigned char*>(data),
                            size) != 1) {
            throw std::runtime_error("Failed to encrypt data");
        }

        // Finalize encryption
        int tmplen;
        if (EVP_EncryptFinal_ex(ctx_,
                               static_cast<unsigned char*>(data) + outlen,
                               &tmplen) != 1) {
            throw std::runtime_error("Failed to finalize encryption");
        }
    }

    void decrypt(void* data, size_t size) {
        if (!data || size == 0) return;

        // Initialize decryption
        if (EVP_DecryptInit_ex(ctx_, EVP_aes_256_cbc(), nullptr,
                             key_.data(), iv_.data()) != 1) {
            throw std::runtime_error("Failed to initialize decryption");
        }

        // Decrypt data
        int outlen;
        if (EVP_DecryptUpdate(ctx_,
                            static_cast<unsigned char*>(data),
                            &outlen,
                            static_cast<const unsigned char*>(data),
                            size) != 1) {
            throw std::runtime_error("Failed to decrypt data");
        }

        // Finalize decryption
        int tmplen;
        if (EVP_DecryptFinal_ex(ctx_,
                               static_cast<unsigned char*>(data) + outlen,
                               &tmplen) != 1) {
            throw std::runtime_error("Failed to finalize decryption");
        }
    }

    void update_key(const std::vector<uint8_t>& key) {
        if (key.size() != 32) {
            throw std::runtime_error("Invalid key size");
        }
        key_ = key;
    }

private:
    std::vector<uint8_t> key_;
    std::vector<uint8_t> iv_{16, 0}; // Initialization vector
    EVP_CIPHER_CTX* ctx_;
};

MemoryEncryption::MemoryEncryption() : impl_(std::make_unique<Impl>()) {}
MemoryEncryption::~MemoryEncryption() = default;

void MemoryEncryption::encrypt(void* data, size_t size) {
    impl_->encrypt(data, size);
}

void MemoryEncryption::decrypt(void* data, size_t size) {
    impl_->decrypt(data, size);
}

void MemoryEncryption::update_key(const std::vector<uint8_t>& key) {
    impl_->update_key(key);
}

} // namespace security
} // namespace x64_vm 