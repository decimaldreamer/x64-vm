#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace x64_vm {
namespace network {

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    // Basic network operations
    bool initialize_network();
    bool shutdown_network();
    bool create_virtual_network();
    bool delete_virtual_network();

    // Network virtualization features
    bool enable_virtual_switch();
    bool enable_virtual_router();
    bool enable_virtual_firewall();
    bool enable_virtual_load_balancer();
    bool enable_virtual_dns();
    bool enable_virtual_dhcp();

    // Network security
    bool enable_network_encryption();
    bool enable_network_isolation();
    bool enable_network_monitoring();
    bool enable_network_traffic_analysis();

    // Network management
    std::string get_network_status();
    std::string get_network_usage();
    std::vector<std::string> get_network_devices();
    bool configure_network_device(const std::string& device_id, const std::string& config);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

} // namespace network
} // namespace x64_vm 