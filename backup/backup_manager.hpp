#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>

namespace x64_vm {
namespace backup {

class BackupManager {
public:
    BackupManager();
    ~BackupManager();

    // Basic backup operations
    bool initialize_backup();
    bool shutdown_backup();
    bool create_backup(const std::string& backup_name);
    bool restore_backup(const std::string& backup_name);

    // Backup scheduling
    bool schedule_backup(const std::chrono::system_clock::time_point& time);
    bool cancel_scheduled_backup();
    bool set_backup_frequency(const std::chrono::hours& interval);

    // Backup management
    bool enable_incremental_backup();
    bool enable_compression();
    bool enable_encryption();
    bool enable_verification();

    // Recovery options
    bool enable_point_in_time_recovery();
    bool enable_automatic_recovery();
    bool enable_remote_recovery();

    // Backup status and information
    std::string get_backup_status();
    std::vector<std::string> list_backups();
    std::string get_backup_details(const std::string& backup_name);
    bool verify_backup_integrity(const std::string& backup_name);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

} // namespace backup
} // namespace x64_vm 