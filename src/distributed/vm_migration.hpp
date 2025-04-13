#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cstdint>

namespace x64_vm {
namespace distributed {

/**
 * @brief Virtual Machine Migration Manager
 * 
 * This class manages the migration of virtual machines from one physical
 * machine to another.
 */
class VMMigration {
public:
    VMMigration();
    ~VMMigration();

    /**
     * @brief Starts the migration process
     * @param source_host Source machine address
     * @param target_host Target machine address
     * @param vm_id ID of the virtual machine to migrate
     */
    void start_migration(const std::string& source_host,
                        const std::string& target_host,
                        uint64_t vm_id);

    /**
     * @brief Checks migration status
     * @return Migration status
     */
    enum class MigrationStatus {
        NOT_STARTED,
        IN_PROGRESS,
        COMPLETED,
        FAILED
    };
    MigrationStatus get_status() const;

    /**
     * @brief Cancels the migration process
     */
    void cancel_migration();

    /**
     * @brief Verifies the migrated machine's state
     * @return Verification result
     */
    bool verify_migration();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace distributed
} // namespace x64_vm 