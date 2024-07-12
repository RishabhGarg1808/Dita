#ifndef DITA_GRAPH_H
#define DITA_GRAPH_H

#include <RawPacket.h>
#include <PcapLiveDevice.h>
#include <iostream>
#include <chrono>
#include "../core/analyzer/Analyzer.h"

using namespace std;
class Graph{
    Analyzer *analyzer;
    struct ServiceStats{
        long int tcpPacketCount = 0;
        long int udpPacketCount = 0;
        long int httpPacketCount = 0;
        long int icmpPacketCount = 0;
        long int sslPacketCount = 0;
        long int sshPacketCount = 0;
        long int totalPacketCount = 0;
        chrono::time_point<chrono::steady_clock> lastUpdate = chrono::steady_clock::now();
    };
public:
    Graph(Analyzer *);
    struct ServiceStats ServiceSt;
    void onPacketArrives(pcpp::RawPacket*,pcpp::PcapLiveDevice* ,void* );
    void consumePacket(pcpp::Packet&);
    struct ServiceStats getServiceStats() const;
    void reset();
};


#endif //DITA_GRAPH_H
