#include "vm_migration.hpp"
#include <boost/asio.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <sstream>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace x64_vm {
namespace distributed {

class VMMigration::Impl {
public:
    Impl() : status_(MigrationStatus::NOT_STARTED) {}

    void start_migration(const std::string& source_host,
                        const std::string& target_host,
                        uint64_t vm_id) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (status_ != MigrationStatus::NOT_STARTED) {
            throw std::runtime_error("Migration already in progress");
        }

        status_ = MigrationStatus::IN_PROGRESS;
        migration_thread_ = std::thread([this, source_host, target_host, vm_id]() {
            try {
                // Connect to source host
                boost::asio::io_context io_context;
                boost::asio::ip::tcp::socket source_socket(io_context);
                boost::asio::ip::tcp::resolver resolver(io_context);
                auto endpoints = resolver.resolve(source_host, "12345");
                boost::asio::connect(source_socket, endpoints);

                // Connect to target host
                boost::asio::ip::tcp::socket target_socket(io_context);
                endpoints = resolver.resolve(target_host, "12345");
                boost::asio::connect(target_socket, endpoints);

                // Send migration request
                std::stringstream request;
                request << "MIGRATE " << vm_id;
                boost::asio::write(target_socket, boost::asio::buffer(request.str()));

                // Receive VM state
                std::vector<char> buffer(1024);
                std::stringstream vm_state;
                size_t bytes_read;
                while ((bytes_read = source_socket.read_some(boost::asio::buffer(buffer))) > 0) {
                    vm_state.write(buffer.data(), bytes_read);
                }

                // Send VM state to target
                boost::asio::write(target_socket, boost::asio::buffer(vm_state.str()));

                // Wait for confirmation
                std::vector<char> response_buffer(1024);
                bytes_read = target_socket.read_some(boost::asio::buffer(response_buffer));
                std::string response(response_buffer.data(), bytes_read);

                if (response == "MIGRATION_COMPLETE") {
                    status_ = MigrationStatus::COMPLETED;
                } else {
                    status_ = MigrationStatus::FAILED;
                }
            } catch (const std::exception& e) {
                status_ = MigrationStatus::FAILED;
            }
            cv_.notify_all();
        });
    }

    MigrationStatus get_status() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return status_;
    }

    void cancel_migration() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (status_ == MigrationStatus::IN_PROGRESS) {
            status_ = MigrationStatus::FAILED;
            if (migration_thread_.joinable()) {
                migration_thread_.join();
            }
        }
    }

    bool verify_migration() {
        std::lock_guard<std::mutex> lock(mutex_);
        return status_ == MigrationStatus::COMPLETED;
    }

private:
    MigrationStatus status_;
    std::thread migration_thread_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
};

VMMigration::VMMigration() : impl_(std::make_unique<Impl>()) {}
VMMigration::~VMMigration() = default;

void VMMigration::start_migration(const std::string& source_host,
                                const std::string& target_host,
                                uint64_t vm_id) {
    impl_->start_migration(source_host, target_host, vm_id);
}

VMMigration::MigrationStatus VMMigration::get_status() const {
    return impl_->get_status();
}

void VMMigration::cancel_migration() {
    impl_->cancel_migration();
}

bool VMMigration::verify_migration() {
    return impl_->verify_migration();
}

} // namespace distributed
} // namespace x64_vm 