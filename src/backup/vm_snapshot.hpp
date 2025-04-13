#pragma once

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <cstdint>

namespace x64_vm {
namespace backup {

/**
 * @brief Virtual Machine Snapshot Manager
 * 
 * This class manages saving and restoring virtual machine states.
 */
class VMSnapshot {
public:
    VMSnapshot();
    ~VMSnapshot();

    /**
     * @brief Takes a snapshot
     * @param vm_id Virtual machine ID
     * @param description Snapshot description
     * @return Snapshot ID
     */
    uint64_t take_snapshot(uint64_t vm_id, const std::string& description);

    /**
     * @brief Restores a snapshot
     * @param snapshot_id Snapshot ID
     */
    void restore_snapshot(uint64_t snapshot_id);

    /**
     * @brief Deletes a snapshot
     * @param snapshot_id Snapshot ID
     */
    void delete_snapshot(uint64_t snapshot_id);

    /**
     * @brief Gets snapshot information
     * @param snapshot_id Snapshot ID
     * @return Snapshot information
     */
    struct SnapshotInfo {
        uint64_t id;
        uint64_t vm_id;
        std::string description;
        std::chrono::system_clock::time_point timestamp;
        uint64_t size;
    };
    SnapshotInfo get_snapshot_info(uint64_t snapshot_id) const;

    /**
     * @brief Lists all snapshots
     * @return Snapshot list
     */
    std::vector<SnapshotInfo> list_snapshots() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace backup
} // namespace x64_vm 