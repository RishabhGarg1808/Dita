#include <iostream>
#include "Analyzer.h"

void Analyzer::icmp_analyze(pcpp::Packet *Packet) {

    if(Packet->isPacketOfType(pcpp::ICMP )
       || Packet->isPacketOfType(pcpp::ICMPv6) ){
        string srcIP = utils->sourceIPExtractor(Packet);
        string destIP = utils->destIPExtractor(Packet);

        // Track both incoming and outgoing ICMP (B5 fix)
        // Incoming: external IP sends to our interface
        if(srcIP != interface_ipv4 && srcIP != interface_ipv6){
            if(checkInMap(srcIP)){
                ping_map[srcIP] = ++ping_map[srcIP];
            }else{
                ping_map[srcIP] = 1;
            }
        }
        // Outgoing: our interface sends to external IP
        else if(destIP != interface_ipv4 && destIP != interface_ipv6){
            if(checkInMap(destIP)){
                ping_map[destIP] = ++ping_map[destIP];
            }else{
                ping_map[destIP] = 1;
            }
        }
    }
}

bool Analyzer::checkInMap(const string& val) {
    auto it = ping_map.find(val);
    if(it != ping_map.end()){
        if(ping_map[val] >= THRESHOLD){
            emit emit_icmp(val);
        }
        return true;
    }
    return false;
}


