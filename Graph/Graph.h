#ifndef DITA_GRAPH_H
#define DITA_GRAPH_H

#include <RawPacket.h>
#include <PcapLiveDevice.h>
#include <iostream>
#include <chrono>
#include "../core/analyzer/protocol_analyzer.h"

class Graph{
    struct ServiceStats{
        long int tcpPacketCount = 0;
        long int udpPacketCount = 0;
        long int httpPacketCount = 0;
        long int icmpPacketCount = 0;
        long int sslPacketCount = 0;
        long int sshPacketCount = 0;
        long int totalPacketCount = 0;
        std::chrono::time_point<std::chrono::steady_clock> lastUpdate = std::chrono::steady_clock::now();
    };

public:
    static struct ServiceStats ServiceSt;
    static void onPacketArrives(pcpp::RawPacket*,pcpp::PcapLiveDevice* ,void* );
    static void consumePacket(pcpp::Packet&);
    static void printtoConsole();
    static struct ServiceStats getServiceStats();
    static void reset();
};


#endif //DITA_GRAPH_H
