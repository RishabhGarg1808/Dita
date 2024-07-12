#include <UdpLayer.h>
#include "Analyzer.h"


void Analyzer::udp_analyze(pcpp::Packet *packet){
    auto* udpLayer = packet->getLayerOfType<pcpp::UdpLayer>();
    if(udpLayer == nullptr){
        return;
    }
    std::string srcIP = utils->sourceIPExtractor(packet);
    std::string dstIP = utils->destIPExtractor(packet);

    std::pair<int,int> portpair = std::make_pair(udpLayer->getUdpHeader()->portSrc,udpLayer->getUdpHeader()->portDst);

    if(srcIP == interface_ipv4 || srcIP == interface_ipv6){
        utils->UDP_update_map(UDP_ConnectionMap_outgoing,dstIP,portpair);
    }else{
        utils->UDP_update_map(UDP_ConnectionMap_incoming,srcIP,portpair);
    }

}