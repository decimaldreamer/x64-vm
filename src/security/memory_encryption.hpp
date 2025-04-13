#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace x64_vm {
namespace security {

/**
 * @brief Memory Encryption Class
 * 
 * This class manages the encryption and decryption of virtual machine memory.
 */
class MemoryEncryption {
public:
    MemoryEncryption();
    ~MemoryEncryption();

    /**
     * @brief Encrypts a memory block
     * @param data Data to encrypt
     * @param size Data size
     */
    void encrypt(void* data, size_t size);

    /**
     * @brief Decrypts an encrypted memory block
     * @param data Data to decrypt
     * @param size Data size
     */
    void decrypt(void* data, size_t size);

    /**
     * @brief Updates the encryption key
     * @param key New key
     */
    void update_key(const std::vector<uint8_t>& key);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace security
} // namespace x64_vm 