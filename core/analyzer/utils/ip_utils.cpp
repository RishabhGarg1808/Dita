#include "Utils.h"
#include "pcapplusplus/IPv4Layer.h"
#include <pcapplusplus/IPv6Layer.h>


string Utils::sourceIPExtractor(pcpp::Packet* Packet){
    pcpp::IPAddress ip;
    if(Packet->isPacketOfType(pcpp::IPv4)){
        auto* IPv4Layer = Packet->getLayerOfType<pcpp::IPv4Layer>();
        ip = IPv4Layer->getSrcIPAddress();
        return ip.toString();
    }
    if(Packet->isPacketOfType(pcpp::IPv6)){
        auto* IPv6Layer = Packet->getLayerOfType<pcpp::IPv6Layer>();
        ip = IPv6Layer->getSrcIPAddress();
        return ip.toString();
    }
    return "";
}

string Utils::destIPExtractor(pcpp::Packet* Packet){
    pcpp::IPAddress ip;
    if(Packet->isPacketOfType(pcpp::IPv4)){
        auto* IPv4Layer = Packet->getLayerOfType<pcpp::IPv4Layer>();
        ip = IPv4Layer->getDstIPAddress();
        return ip.toString();
    }
    if(Packet->isPacketOfType(pcpp::IPv6)){
        auto* IPv6Layer = Packet->getLayerOfType<pcpp::IPv6Layer>();
        ip = IPv6Layer->getDstIPAddress();
        return ip.toString();
    }

    return "";
}
