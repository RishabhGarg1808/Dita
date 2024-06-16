#include "Graph.h"
struct Graph::ServiceStats Graph::ServiceSt = {0};
void Graph::onPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie) {
    auto* stats = (ServiceStats*)cookie;
    pcpp::Packet parsedPacket(packet);
    consumePacket(parsedPacket);
    packetSwitcher(&parsedPacket);
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
    if(packet.isPacketOfType(pcpp::SSL))
        ServiceSt.sslPacketCount++;
    if(packet.isPacketOfType(pcpp::SSH))
        ServiceSt.sshPacketCount++;
}

Graph::ServiceStats Graph::getServiceStats() {
    return Graph::ServiceSt;
}

void Graph::printtoConsole() {
    std::cout << "TCP Packets: " << ServiceSt.tcpPacketCount << std::endl;
    std::cout << "UDP Packets: " << ServiceSt.udpPacketCount << std::endl;
    std::cout << "HTTP Packets: " << ServiceSt.httpPacketCount << std::endl;
    std::cout << "ICMP Packets: " << ServiceSt.icmpPacketCount << std::endl;
    std::cout << "SSL Packets: " << ServiceSt.sslPacketCount << std::endl;
    std::cout << "SSH Packets: " << ServiceSt.sshPacketCount << std::endl;
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
