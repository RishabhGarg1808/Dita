#include "protocol_analyzer.h"

std::string getProtocolAsString(pcpp::ProtocolType ProtocolType){
    switch(ProtocolType){
        case pcpp::Ethernet:
            return "Ethernet";
        case pcpp::ARP:
            return "ARP";
        case pcpp::ICMP:
            return "ICMP";
        case pcpp::IPv4:
            return "IPv4";
        case pcpp::IPv6:
            return "IPv6";
        case pcpp::HTTP:
            return "HTTP";
        case pcpp::TCP:
            return "TCP";
        case pcpp::UDP:
            return "UDP";
        case pcpp::SSH:
            return "SSH";
    }
    return "";
}

void packetSwitcher(pcpp::Packet *packet){
    if (packet->isPacketOfType(pcpp::IPv4)) {
        ipv4_analyze(packet);
    }
    if (packet->isPacketOfType(pcpp::IPv6)) {
        ipv6_analyze(packet);
    }
    if(packet->isPacketOfType(pcpp::SSH)) {
        sshAnalyze(packet);
    }
    if(packet->isPacketOfType(pcpp::TCP)){
        tcp_analyze(packet);
    }
    if(packet->isPacketOfType(pcpp::UDP)){
        udp_analyze(packet);
    }
}
