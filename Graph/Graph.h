#ifndef DITA_GRAPH_H
#define DITA_GRAPH_H

#include <RawPacket.h>
#include <PcapLiveDevice.h>
#include <iostream>
#include <chrono>
#include <atomic>

#include "core/analyzer/Analyzer.h"

using namespace std;

class Graph{
    Analyzer *analyzer;

    // Cumulative counters, incremented on the capture thread.
    std::atomic<long long> tcpCount{0};
    std::atomic<long long> udpCount{0};
    std::atomic<long long> httpCount{0};
    std::atomic<long long> icmpCount{0};
    std::atomic<long long> sslCount{0};
    std::atomic<long long> sshCount{0};
    std::atomic<long long> totalCount{0};

    // Baseline for rate sampling (touched only on the GUI thread).
    long long lastTcp = 0, lastUdp = 0, lastHttp = 0, lastIcmp = 0;
    long long lastSsl = 0, lastSsh = 0, lastTotal = 0;
    std::chrono::steady_clock::time_point lastSampleTime{};
    bool hasBaseline = false;

public:
    // Cumulative counters since capture started.
    struct ServiceStats{
        long long tcpPacketCount = 0;
        long long udpPacketCount = 0;
        long long httpPacketCount = 0;
        long long icmpPacketCount = 0;
        long long sslPacketCount = 0;
        long long sshPacketCount = 0;
        long long totalPacketCount = 0;
    };

    // Packets per second since the previous sampleRates() call.
    struct ServiceRates{
        double tcp = 0, udp = 0, http = 0, icmp = 0, ssl = 0, ssh = 0, total = 0;
    };

    Graph(Analyzer *analyze);
    ServiceStats getServiceStats() const;
    ServiceRates sampleRates();
    void onPacketArrives(pcpp::RawPacket*,pcpp::PcapLiveDevice* ,void* );
    void consumePacket(pcpp::Packet&);
};

#endif //DITA_GRAPH_H
