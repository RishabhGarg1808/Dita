#include <IPv4Layer.h>
#include "udp_analyzer.h"
#include "utils/defines.h"
#include "utils/ip_utils.h"
#include "utils/map_util.h"

map<string ,pair<int,int>> UDP_ConnectionMap_incoming;
map<string ,pair<int,int>> UDP_ConnectionMap_outgoing;

void udp_analyze(pcpp::Packet *packet){
    auto* udpLayer = packet->getLayerOfType<pcpp::UdpLayer>();
    if(udpLayer == nullptr){
        return;
    }
    std::string srcIP = sourceIPExtractor(packet);
    std::string dstIP = destIPExtractor(packet);

    std::pair<int,int> portpair = std::make_pair(udpLayer->getUdpHeader()->portSrc,udpLayer->getUdpHeader()->portDst);

    if(srcIP == interface_ipv4 || srcIP == interface_ipv6){
        UDP_update_map(UDP_ConnectionMap_outgoing,dstIP,portpair);
    }else{
        UDP_update_map(UDP_ConnectionMap_incoming,srcIP,portpair);
    }

}