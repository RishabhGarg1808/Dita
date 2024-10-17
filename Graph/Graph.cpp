#include "Graph.h"


void Graph::onPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie) {
    auto* stats = (ServiceStats*)cookie;
    pcpp::Packet parsedPacket(packet);
    consumePacket(parsedPacket);
    analyzer->packetSwitcher(&parsedPacket);
}

void Graph::consumePacket(pcpp::Packet &packet) {
    ServiceSt.totalPacketCount++;
    if(packet.isPacketOfType(pcpp::TCP))
        ServiceSt.tcpPacketCount++;
    if(packet.isPacketOfType(pcpp::UDP))
        ServiceSt.udpPacketCount++;
    if(packet.isPacketOfType(pcpp::HTTP))
        ServiceSt.httpPacketCount++;
    if(packet.isPacketOfType(pcpp::ICMP))
        ServiceSt.icmpPacketCount++;
    if(packet.isPacketOfType(pcpp::ICMPv6))
        ServiceSt.icmpPacketCount++;
    if(packet.isPacketOfType(pcpp::SSL))
        ServiceSt.sslPacketCount++;
    if(packet.isPacketOfType(pcpp::SSH))
        ServiceSt.sshPacketCount++;
}

Graph::ServiceStats Graph::getServiceStats() const {
    return ServiceSt;
}

void Graph::reset() {
    ServiceSt.tcpPacketCount = 0;
    ServiceSt.udpPacketCount = 0;
    ServiceSt.httpPacketCount = 0;
    ServiceSt.icmpPacketCount = 0;
    ServiceSt.sslPacketCount = 0;
    ServiceSt.sshPacketCount = 0;
    ServiceSt.totalPacketCount = 0;
}

Graph::Graph(Analyzer *analyze) {
    this->analyzer = analyze;
    this->reset();
}