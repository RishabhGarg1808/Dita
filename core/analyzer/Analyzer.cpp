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
    mapLock.lockForWrite();
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
    mapLock.unlock();
}

void Analyzer::setInterfaceIpv4(const string &interfaceIpv4) {
    // Guarded by the same lock the analyzers run under (packetSwitcher
    // holds it), so the capture thread never reads these while they are
    // being reassigned.
    mapLock.lockForWrite();
    interface_ipv4 = interfaceIpv4;
    mapLock.unlock();
}

void Analyzer::setInterfaceIpv6(const string &interfaceIpv6) {
    mapLock.lockForWrite();
    interface_ipv6 = interfaceIpv6;
    mapLock.unlock();
}

const map<pair<string, string>, pair<int, int>> &Analyzer::getTcpConnectionMapIncoming() const {
    mapLock.lockForRead();
    return TCP_ConnectionMap_incoming;
}

const map<pair<string, string>, pair<int, int>> &Analyzer::getTcpConnectionMapOutgoing() const {
    mapLock.lockForRead();
    return TCP_ConnectionMap_outgoing;
}

const map<string, pair<int, int>> &Analyzer::getUdpConnectionMapIncoming() const {
    mapLock.lockForRead();
    return UDP_ConnectionMap_incoming;
}

const map<string, pair<int, int>> &Analyzer::getUdpConnectionMapOutgoing() const {
    mapLock.lockForRead();
    return UDP_ConnectionMap_outgoing;
}

void Analyzer::setSshConnectionMap(const map<std::string, std::string> &sshConnectionMap) {
    mapLock.lockForWrite();
    SSH_ConnectionMap = sshConnectionMap;
    mapLock.unlock();
}

void Analyzer::cleanupDeadConnections() {
    mapLock.lockForWrite();
    // M4: Remove dead TCP connections to prevent unbounded growth
    auto it = TCP_ConnectionMap_incoming.begin();
    while (it != TCP_ConnectionMap_incoming.end()) {
        if (it->second.second == 1) { // Dead status
            it = TCP_ConnectionMap_incoming.erase(it);
        } else {
            ++it;
        }
    }
    it = TCP_ConnectionMap_outgoing.begin();
    while (it != TCP_ConnectionMap_outgoing.end()) {
        if (it->second.second == 1) { // Dead status
            it = TCP_ConnectionMap_outgoing.erase(it);
        } else {
            ++it;
        }
    }
    mapLock.unlock();
}

// Thread-safe copy methods
map<pair<string,string>, pair<int,int>> Analyzer::getTcpIncomingCopy() const {
    mapLock.lockForRead();
    auto copy = TCP_ConnectionMap_incoming;
    mapLock.unlock();
    return copy;
}

map<pair<string,string>, pair<int,int>> Analyzer::getTcpOutgoingCopy() const {
    mapLock.lockForRead();
    auto copy = TCP_ConnectionMap_outgoing;
    mapLock.unlock();
    return copy;
}

map<string, pair<int,int>> Analyzer::getUdpIncomingCopy() const {
    mapLock.lockForRead();
    auto copy = UDP_ConnectionMap_incoming;
    mapLock.unlock();
    return copy;
}

map<string, pair<int,int>> Analyzer::getUdpOutgoingCopy() const {
    mapLock.lockForRead();
    auto copy = UDP_ConnectionMap_outgoing;
    mapLock.unlock();
    return copy;
}

map<string, string> Analyzer::getSshCopy() const {
    mapLock.lockForRead();
    auto copy = SSH_ConnectionMap;
    mapLock.unlock();
    return copy;
}
