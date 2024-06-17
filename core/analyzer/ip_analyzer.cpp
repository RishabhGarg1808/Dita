#include "ip_analyzer.h"
#include "utils/defines.h"
#include "utils/map_util.h"
#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/IPv6Layer.h"

std::map<std::string ,int> ipCount;
std::multimap<int , std::string> SortedIPs;



int DROPPED_IPv4=0;
int DROPPED_IPv6=0;

void ipv4_analyze(pcpp::Packet *Packet){
    auto* iPv4Layer = Packet->getLayerOfType<pcpp::IPv4Layer>();
    if(iPv4Layer == NULL){
        DROPPED_IPv4++;
    }else{
        if(iPv4Layer->getSrcIPv4Address().toString() != interface_ipv4){
            IP_update_map(ipCount, iPv4Layer->getSrcIPv4Address().toString());
        }
    }
}

void ipv6_analyze(pcpp::Packet *Packet){

    auto* iPv6Layer = Packet->getLayerOfType<pcpp::IPv6Layer>();
    if(iPv6Layer == NULL){
        DROPPED_IPv6++;
    }else{
        if(iPv6Layer->getSrcIPv6Address().toString() != interface_ipv6){
            IP_update_map(ipCount, iPv6Layer->getSrcIPv6Address().toString());
        }
    }
}

void sortIPs(){
    SortedIPs.clear();
    std::vector<std::pair<std::string, int>> vec(ipCount.begin(), ipCount.end());
    std::sort(vec.begin(), vec.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b){
        return a.second > b.second;
    });
    for(auto& pair : vec){
        SortedIPs.insert(std::make_pair(pair.second, pair.first));
    }
}




