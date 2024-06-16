#include "ip_utils.h"

std::string sourceIPExtractor(pcpp::Packet* Packet){
    pcpp::IPAddress ip;
    if(Packet->isPacketOfType(pcpp::IPv4)){
        auto* IPv4Layer = Packet->getLayerOfType<pcpp::IPv4Layer>();
        ip = IPv4Layer->getSrcIPAddress();
        return ip.toString();
    }
    if(Packet->isPacketOfType(pcpp::IPv6)){
        auto* IPv6Layer = Packet->getLayerOfType<pcpp::IPv4Layer>();
        ip = IPv6Layer->getSrcIPAddress();
        return ip.toString();
    }
}

std::string destIPExtractor(pcpp::Packet* Packet){
    pcpp::IPAddress ip;
    if(Packet->isPacketOfType(pcpp::IPv4)){
        auto* IPv4Layer = Packet->getLayerOfType<pcpp::IPv4Layer>();
        ip = IPv4Layer->getDstIPAddress();
        return ip.toString();
    }
    if(Packet->isPacketOfType(pcpp::IPv6)){
        auto* IPv6Layer = Packet->getLayerOfType<pcpp::IPv4Layer>();
        ip = IPv6Layer->getDstIPAddress();
        return ip.toString();
    }
}
