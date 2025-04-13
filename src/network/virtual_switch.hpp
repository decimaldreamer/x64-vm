#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace x64_vm {
namespace network {

/**
 * @brief Virtual Switch Class
 * 
 * This class manages network communication between virtual machines
 * and enforces firewall rules.
 */
class VirtualSwitch {
public:
    VirtualSwitch();
    ~VirtualSwitch();

    /**
     * @brief Initializes the virtual switch
     * @param config Configuration parameters
     */
    void initialize(const std::unordered_map<std::string, std::string>& config);

    /**
     * @brief Processes a packet
     * @param source_mac Source MAC address
     * @param dest_mac Destination MAC address
     * @param packet Packet data
     */
    void process_packet(const std::string& source_mac,
                       const std::string& dest_mac,
                       const std::vector<uint8_t>& packet);

    /**
     * @brief Adds a firewall rule
     * @param rule Rule
     */
    void add_firewall_rule(const std::string& rule);

    /**
     * @brief Adds a QoS rule
     * @param rule Rule
     */
    void add_qos_rule(const std::string& rule);

    /**
     * @brief Monitors network traffic
     * @return Traffic statistics
     */
    struct TrafficStats {
        uint64_t packets_sent;
        uint64_t packets_received;
        uint64_t bytes_sent;
        uint64_t bytes_received;
        std::unordered_map<std::string, uint64_t> protocol_stats;
    };
    TrafficStats get_traffic_stats() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace network
} // namespace x64_vm 