#include "Graph.h"

void Graph::onPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie) {
    // `this` is the Graph the callback was bound to in DevHandler::start_capture.
    // (The cookie is informational; member access goes through `this`.)
    pcpp::Packet parsedPacket(packet);
    consumePacket(parsedPacket);
    analyzer->packetSwitcher(&parsedPacket);
}

void Graph::consumePacket(pcpp::Packet &packet) {
    totalCount++;
    if(packet.isPacketOfType(pcpp::TCP))
        tcpCount++;
    if(packet.isPacketOfType(pcpp::UDP))
        udpCount++;
    if(packet.isPacketOfType(pcpp::HTTP))
        httpCount++;
    if(packet.isPacketOfType(pcpp::ICMP))
        icmpCount++;
    if(packet.isPacketOfType(pcpp::ICMPv6))
        icmpCount++;
    if(packet.isPacketOfType(pcpp::SSL))
        sslCount++;
    if(packet.isPacketOfType(pcpp::SSH))
        sshCount++;
}

Graph::ServiceStats Graph::getServiceStats() const {
    ServiceStats s;
    s.tcpPacketCount = tcpCount.load();
    s.udpPacketCount = udpCount.load();
    s.httpPacketCount = httpCount.load();
    s.icmpPacketCount = icmpCount.load();
    s.sslPacketCount = sslCount.load();
    s.sshPacketCount = sshCount.load();
    s.totalPacketCount = totalCount.load();
    return s;
}

Graph::ServiceRates Graph::sampleRates() {
    ServiceRates rates;
    auto now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration<double>(now - lastSampleTime).count();

    long long tcp = tcpCount.load();
    long long udp = udpCount.load();
    long long http = httpCount.load();
    long long icmp = icmpCount.load();
    long long ssl = sslCount.load();
    long long ssh = sshCount.load();
    long long total = totalCount.load();

    if (!hasBaseline || dt <= 0) {
        // First sample: just establish the baseline.
        hasBaseline = true;
    } else {
        rates.tcp = (tcp - lastTcp) / dt;
        rates.udp = (udp - lastUdp) / dt;
        rates.http = (http - lastHttp) / dt;
        rates.icmp = (icmp - lastIcmp) / dt;
        rates.ssl = (ssl - lastSsl) / dt;
        rates.ssh = (ssh - lastSsh) / dt;
        rates.total = (total - lastTotal) / dt;
    }

    lastTcp = tcp;
    lastUdp = udp;
    lastHttp = http;
    lastIcmp = icmp;
    lastSsl = ssl;
    lastSsh = ssh;
    lastTotal = total;
    lastSampleTime = now;
    return rates;
}

Graph::Graph(Analyzer *analyze) {
    this->analyzer = analyze;
}
