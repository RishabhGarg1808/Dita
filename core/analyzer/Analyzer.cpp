#include "Analyzer.h"

string Analyzer::getProtocolAsString(pcpp::ProtocolType ProtocolType){
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

void Analyzer::packetSwitcher(pcpp::Packet *packet){
    if(packet->isPacketOfType(pcpp::SSH)) {
        sshAnalyze(packet);
    }
    if(packet->isPacketOfType(pcpp::TCP)){
        tcp_analyze(packet);
    }
    if(packet->isPacketOfType(pcpp::UDP)){
        udp_analyze(packet);
    }
    if(packet->isPacketOfType(pcpp::ICMP) ||
       packet->isPacketOfType(pcpp::ICMPv6)){
        icmp_analyze(packet);
    }
}

void Analyzer::setInterfaceIpv4(const string &interfaceIpv4) {
    interface_ipv4 = interfaceIpv4;
}

void Analyzer::setInterfaceIpv6(const string &interfaceIpv6) {
    interface_ipv6 = interfaceIpv6;
}

const map<pair<string, string>, pair<int, int>> &Analyzer::getTcpConnectionMapIncoming() const {
    return TCP_ConnectionMap_incoming;
}

const map<pair<string, string>, pair<int, int>> &Analyzer::getTcpConnectionMapOutgoing() const {
    return TCP_ConnectionMap_outgoing;
}

const map<string, pair<int, int>> &Analyzer::getUdpConnectionMapIncoming() const {
    return UDP_ConnectionMap_incoming;
}

const map<string, pair<int, int>> &Analyzer::getUdpConnectionMapOutgoing() const {
    return UDP_ConnectionMap_outgoing;
}

void Analyzer::setSshConnectionMap(const map<std::string, std::string> &sshConnectionMap) {
    SSH_ConnectionMap = sshConnectionMap;
}

